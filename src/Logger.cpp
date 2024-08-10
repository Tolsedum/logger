#include "Logger.hpp"

FileBuffer::FileBuffer(
    std::string file_name,
    bool create_file_if_not_exists
)
    : file_name_(file_name)
    , create_file_if_not_exists_(create_file_if_not_exists)
{
    if(create_file_if_not_exists_){
        if(!ufn::createFileAndDirrs(file_name_)){
            std::string error =
                ufn::error_in_function_create_file_and_dir.what();
            if(!error.empty()){
                throw std::runtime_error(error);
            }
        }
    }else if(!std::filesystem::exists(file_name_)){
        throw std::runtime_error("File is not exists " + file_name_);
    }
    std::ifstream f_point(file_name_);
    if(f_point.is_open()){
        std::string str;
        while (!f_point.eof()){
            std::getline(f_point, str);
            line_count_++;
        }
        f_point.close();
    }else{
        throw std::runtime_error("Cent open file " + file_name_);
    }
}

FileBuffer::~FileBuffer(){
    if(!buffer_.empty()){
        buffer_.clear();
    }
    if(!file_name_.empty()){
        file_name_.clear();
    }
}

bool FileBuffer::putBufferMessage(
    std::string message, unsigned buffer_size
){
    buffer_.push_back(message);
    if(buffer_.size() == buffer_size){
        return true;
    }
    return false;
}

void Logger::collectFileInfo(std::string file_name, unsigned f_size){
    if(size_file_to_zip_ != 0 && f_size >= size_file_to_zip_){
        auto search = file_info_.find(file_name);
        if(search == file_info_.end()){
            file_info_.insert(std::pair<std::string, FileInfo>(
                file_name, {file_name, f_size, true}
            ));
        }else{
            search->second.in_archive_ = true;
        }
    }
}

int Logger::getMaxNum(const std::string &file_name){
    std::string parent_path = ufn::getParentDir(file_name);
    int count = 0;
    for (auto &&path_ :
        std::filesystem::directory_iterator(
            parent_path
    )){
        std::string file_n = path_.path().stem();
        std::size_t pos = file_n.find_last_of("_");
        if(pos != std::string::npos && path_.path().extension() == ".zip"){
            count++;
        }
    }
    return count;
}

void Logger::deleteFileIftimesUp(
    const std::string &dir_path,
    const std::string &template_name
){

    for (auto &&file_name_in_path
        : std::filesystem::directory_iterator(dir_path)
    ){
        std::string s_name_file = file_name_in_path.path()
            .string();
        std::string end_file_name("_");
        end_file_name.append(template_name).append(".zip");
        std::size_t pos = s_name_file
            .find(end_file_name);
        if(pos != std::string::npos){
            unsigned long current_time = std::time(nullptr);
            unsigned long file_time =
                ufn::getFileCreationDate(s_name_file);
            bool remove_file = time_live_zip_files_ != 0
                && time_live_zip_files_ <= (current_time
                    - file_time);
            if(remove_file){
                remove(s_name_file.c_str());
            }
        }
    }
}

void Logger::archivingLogFiles(){
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto &&file_info : file_info_){

        std::string file_name = file_info.first;
        std::string template_file_name =
            std::filesystem::path(file_name).stem();
        size_t pos = template_file_name.find_last_of("_");
        if(pos != std::string::npos){
            template_file_name.erase(pos);
        }
        std::cout << template_file_name << std::endl;
        deleteFileIftimesUp(
            ufn::getParentDir(file_name),
            template_file_name
        );

        std::filesystem::path path(file_name);
        std::string file_name_zip = path.parent_path();

        file_name_zip
            .append("/")
            .append(ufn::currentDateTime())
            .append("_")
            .append(template_file_name)
            .append(".zip");
        archive::Archiver zip_archive(
            file_name_zip,
            0,
            true
        );
        zip_archive.addFile(file_name, path.filename());
        zip_archive.save();
        std::cout << file_name << std::endl;
        remove(file_name.c_str());
    }
}

void Logger::flashToFile(
    std::string name_file, std::vector<std::string>& list
){
    std::lock_guard<std::mutex> lock(mutex_);
    std::ofstream f(name_file, std::ios::app);

    if(f.is_open()){
        for(std::string message : list){
            message.append(1, '\n');
            f.write(message.c_str(), message.size());
        }
        list.clear();
        int f_size = f.tellp();
        f.close();
        collectFileInfo(name_file, f_size);
    }
}

void Logger::log(
    const std::string message,
    const std::string name_file,
    log_level level,
    bool create_file_if_not_exists
){
    std::string str = log_level_str[level]
        + " ["
        + ufn::currentDateTime()
        + "]: "
        + message;

    auto search = buffer_.find(name_file);
    if(search == buffer_.end()){
        FileBuffer buf_(
            name_file,
            create_file_if_not_exists_ || create_file_if_not_exists
        );
        buffer_[name_file] = buf_;
        auto data = buffer_.insert({name_file, buf_});
        if(!data.second){
            search = data.first;
        }else{
            throw std::runtime_error("Cant find element in buffer");
        }
    }

    if(search->second.putBufferMessage(str, buffer_size_)){
        flashToFile(name_file, search->second.getBuffer());
        buffer_.erase(search);
    }
}

void Logger::flash(){
    if(!buffer_.empty()){
        for(auto buf : buffer_){
            flashToFile(buf.first, buf.second.getBuffer());
        }
        buffer_.clear();
    }
    archivingLogFiles();
}
