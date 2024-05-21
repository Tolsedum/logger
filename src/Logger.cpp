#include "Logger.hpp"

FileBuffer::FileBuffer(
    std::string file_name,
    bool create_file_if_not_exists
)
    : file_name_(file_name)
    , create_file_if_not_exists_(create_file_if_not_exists)
{
    if(create_file_if_not_exists_){
        ufn::createFileAndDirrs(file_name_);
    }else if(!std::filesystem::exists(file_name_)){
        throw std::runtime_error("File is not exists " + file_name_);
    }
    std::ifstream f(file_name_);
    std::string str;
        while (!f.eof()){
        std::getline(f, str);
        line_count_++;
    }
    f.close();
}

bool FileBuffer::putBufferMessage(
    std::string message, std::size_t buffer_size_
){
    buffer_.push_back(message);
    if(buffer_.size() == buffer_size_){
        return true;
    }
    return false;
}

void Logger::flashToFile(std::string name_file, std::vector<std::string>& list){
    std::lock_guard<std::mutex> lock(mutex_);
    std::ofstream f(name_file, std::ios::app);

    if(f.is_open()){
        for(std::string message : list){
            message.append(1, '\n');
            f.write(message.c_str(), message.size());
        }
        list.clear();
        f.close();
    }
}

void Logger::log(
    const std::string message,
    const std::string name_file,
    log_level level,
    bool create_file_if_not_exists
){
    if(create_file_if_not_exists_ || create_file_if_not_exists){
        if(!ufn::createFileAndDirrs(name_file)){
            std::string error = ufn::error_in_function_create_file_and_dir.what();
            if(!error.empty()){
                throw std::runtime_error(error);
            }
        }
    }else if(!std::filesystem::exists(name_file)){
        throw std::runtime_error("File is not exists " + name_file);
    }
    std::string str = log_level_str[level]
        + " ["
        + ufn::currentDateTime()
        + "]: "
        + message;

    auto search = buffer_.find(name_file);
    if(search != buffer_.end()){
        FileBuffer &buf = buffer_[name_file];
        if(buf.putBufferMessage(str, buffer_size_)){
            flashToFile(name_file, buf.getBuffer());
            buffer_.erase(search);
        }
    }else{
        FileBuffer buf(name_file, create_file_if_not_exists_);
        buf.putBufferMessage(str, buffer_size_);
        buffer_[name_file] = buf;
    }
}

void Logger::flash(){
    if(!buffer_.empty()){
        for(auto buf : buffer_){
            flashToFile(buf.first, buf.second.getBuffer());
        }
    }
    buffer_.clear();
}
