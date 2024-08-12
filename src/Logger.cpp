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

void Logger::collectFileInfo(
    std::string file_name,
    unsigned long f_size
){
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

void Logger::deleteFileIfTimesUp(
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
        std::size_t pos = s_name_file.find(end_file_name);

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
    for (auto &&file_info : file_info_){

        std::string file_name = file_info.first;
        std::string template_file_name =
            std::filesystem::path(file_name).stem();
        size_t pos = template_file_name.find_last_of("_");
        if(pos != std::string::npos){
            template_file_name.erase(pos);
        }

        deleteFileIfTimesUp(
            ufn::getParentDir(file_name),
            template_file_name
        );

        std::filesystem::path path(file_name);
        std::string parent_path =
            std::filesystem::path(file_name).parent_path();

        std::string file_name_zip = getNameFileForArchive(
            parent_path, template_file_name
        );

        if(std::filesystem::exists(file_name_zip)){
            std::cout << "file exists: "<< file_name_zip<<std::endl;
            throw "file exists";
        }
        archive::Archiver zip_archive(
            file_name_zip,
            0,
            true
        );
        zip_archive.addFile(file_name, path.filename());
        zip_archive.save();
        remove(file_name.c_str());
    }
    file_info_.clear();
}

std::string Logger::getNameFileForArchive(
    std::string &parent_path, std::string &template_file_name
){
    std::string file_name_zip = parent_path;

    bool set = true;
    short count = 0;
    short max_count = 256;
    while (set){
        std::string tmp{file_name_zip};
        tmp.append("/")
            .append(ufn::currentDateTime())
            .append("_")
            .append(template_file_name)
            .append(".zip");
        if(!std::filesystem::exists(tmp)){
            file_name_zip = tmp;
            set = false;
            break;
        }
        count++;
        if(max_count == count){
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if(set){
        // ToDo Do some
        std::cout
            << "File exists !!!!!!!!!!!!!!!!!!!!!!!!!"
        << std::endl;
    }
    return file_name_zip;
}

void Logger::flashToFile(
    std::string name_file, std::vector<std::string>& list
){
    static int count = 0;
    count++;
    std::cout << "count: " << count << std::endl;

    if(list.empty()){
        return;
    }
    std::ofstream f(name_file, std::ios::app);
    if(f.is_open()){
        for(std::string message : list){
            if(message.empty()){
                continue;
            }
            message.append(1, '\n');
            f.write(message.c_str(), message.size());
        }
        list.clear();
        unsigned long f_size = f.tellp();
        f.close();
        collectFileInfo(name_file, f_size);
    }else{
        throw "Cant open file: " + name_file;
    }
}

void Logger::insertInLog(
    const std::string message,
    const std::string name_file,
    log_level level,
    bool create_file_if_not_exists
){
    if(name_file.empty()){
        return;
    }

    std::map<std::string, FileBuffer>::iterator search =
        buffer_.find(name_file);

    if(search == buffer_.end()){
        FileBuffer buf_(
            name_file,
            create_file_if_not_exists
        );
        auto [search__, success] = buffer_.insert({name_file, buf_});
        if(!success){
            throw std::runtime_error("Cant insert in buffer");
        }
        search = search__;
    }

    std::string str = log_level_str[level]
        + " ["
        + ufn::currentDateTime()
        + "]: "
        + message;

    if(search->second.putBufferMessage(str, buffer_size_)
        && !is_atomic_
    ){
        flashToFile(name_file, search->second.getBuffer());
    }
}

void Logger::log(
    const std::string message,
    const std::string name_file,
    log_level level
){
    insertInLog(
        message,
        name_file,
        level,
        create_file_if_not_exists_
    );
}

void Logger::log(
    const std::string message,
    const std::string name_file,
    log_level level,
    bool create_file_if_not_exists
){
    insertInLog(
        message,
        name_file,
        level,
        create_file_if_not_exists
    );
}

void Logger::flash(){
    if(!buffer_.empty()){
        for(auto buf : buffer_){
            std::vector<std::string> *buf_p =
                &buf.second.getBuffer();
            flashToFile(buf.first, *buf_p);
        }
        buffer_.clear();
    }
    archivingLogFiles();
}
