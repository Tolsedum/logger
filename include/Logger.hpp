#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <array>
#include <exception>
#include <mutex>
#include "functions.hpp"

/**
 * @brief Buffer
 * @author Tolsedum
 */
struct FileBuffer{
private:
    std::string file_name_;
    std::vector<std::string> buffer_;
    std::size_t line_count_;
    bool create_file_if_not_exists_;
public:
    FileBuffer(bool create_file_if_not_exists = false)
        : create_file_if_not_exists_(create_file_if_not_exists = false)
    {};
    FileBuffer(std::string file_name, bool create_file_if_not_exists);
    ~FileBuffer(){};

    std::string getFileName(){return file_name_;}
    std::vector<std::string> &getBuffer(){return buffer_;}
    std::size_t getLineCoutn(){return line_count_;}
    /**
     * @brief If crowded return true else false
     *
     * @param message
     * @param buffer_size_
     * @return true
     * @return false
     */
    bool putBufferMessage(std::string message, std::size_t buffer_size_);
};

enum log_level { error, warning, critical, debug };
inline static std::array<std::string, log_level::debug + 1u> log_level_str = {
    "ERROR", "WARNING", "CRITICAL", "DEBUG"
};

/**
 * @brief Logger object
 * @author Tolsedum
 */
class Logger{
private:
    std::map<std::string, FileBuffer> buffer_;
    std::size_t buffer_size_;
    std::size_t file_line_count_;
    std::mutex mutex_;
    bool create_file_if_not_exists_;

    void flashToFile(std::string name_file, std::vector<std::string>& list);
public:
    Logger(
        std::size_t buffer_size = 10u,
        std::size_t file_line_count  = 1000u,
        bool create_file_if_not_exists = false
    )
        : buffer_size_(buffer_size)
        , file_line_count_(file_line_count)
        , create_file_if_not_exists_(create_file_if_not_exists)
    {};
    ~Logger(){flash();};

    void log(
        const std::string message,
        const std::string name_file,
        log_level level = log_level::debug,
        bool create_file_if_not_exists = false
    );

    void flash();
};

#endif /* LOGGER_HPP */
