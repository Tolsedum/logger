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

void Logger::archivingLogFiles(){
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto &&file_info : file_info_){
        std::string file_name = file_info.first;
        std::string parent_path = ufn::getParentDir(file_name);
        // int count = 0;
        std::map<std::string, std::string> rename_list;
        for (auto &&path_ :
            std::filesystem::directory_iterator(
                parent_path
        )){

            std::string rename_file{path_.path().parent_path()};
            std::string file_n =
                path_.path().stem();
            std::size_t pos = file_n.find_last_of("_");
            if(pos != std::string::npos){
                int num = ufn::strToInt(file_n.substr(pos+1));
                file_n.erase(pos);
                rename_file.append("/")
                    .append(file_n)
                    .append("_")
                    .append(std::to_string(++num))
                    .append(".zip");
            }else{
                rename_file.append("/")
                    .append(file_n)
                    .append("_1")
                    .append(".zip");
            }

            pos = file_name.find(file_n);

            if(std::string::npos != pos
                && path_.path().extension() == ".zip"
            ){
                std::string zip_file_name_{path_.path()};

                if(time_live_zip_files_ != 0){
                    time_t create_file_date =
                        ufn::getFileCreationDate(zip_file_name_);
                    time_t current_time = std::time(nullptr);

                    if(
                        time_live_zip_files_ <=
                            current_time - create_file_date
                    ){
                        remove(zip_file_name_.c_str());
                    }
                }
                if(!rename_file.empty()
                    && std::filesystem::exists(zip_file_name_
                )){
                    rename(
                        zip_file_name_.c_str(),
                        rename_file.c_str()
                    );
                }
            }
        }

        std::filesystem::path path(file_name);
        std::string file_name_zip = path.parent_path();
        file_name_zip.append("/").append(path.stem());
        // if(count > 0){
        //     file_name_zip
        //         .append("_")
        //         .append(std::to_string(count));
        // }

        archive::Archiver zip_archive(file_name_zip + ".zip");
        zip_archive.addFile(file_name, path.filename());
        zip_archive.save();
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
