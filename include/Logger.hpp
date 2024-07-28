#ifndef LOGGER_HPP
#define LOGGER_HPP
/** Version 1 */

/**
 *  __________________________________________
 * |                                          |
 * |   ╭━━━━┳━━━┳╮╱╱╭━━━┳━━━┳━━━┳╮╱╭┳━╮╭━╮    |
 * |   ┃╭╮╭╮┃╭━╮┃┃╱╱┃╭━╮┃╭━━┻╮╭╮┃┃╱┃┃┃╰╯┃┃    |
 * |   ╰╯┃┃╰┫┃╱┃┃┃╱╱┃╰━━┫╰━━╮┃┃┃┃┃╱┃┃╭╮╭╮┃    |
 * |   ╱╱┃┃╱┃┃╱┃┃┃╱╭╋━━╮┃╭━━╯┃┃┃┃┃╱┃┃┃┃┃┃┃    |
 * |   ╱╱┃┃╱┃╰━╯┃╰━╯┃╰━╯┃╰━━┳╯╰╯┃╰━╯┃┃┃┃┃┃    |
 * |   ╱╱╰╯╱╰━━━┻━━━┻━━━┻━━━┻━━━┻━━━┻╯╰╯╰╯    |
 * |__________________________________________|
 * |                                          |
 * | Permission is hereby granted, free of    |
 * | charge, to any person obtaining a copy of|
 * | of this software and accompanying files, |
 * | to use them without restriction,         |
 * | including, without limitation, the       |
 * | rights to use, copy, modify, merge,      |
 * | publish, distribute, sublicense and/or   |
 * | sell copies of the software. The authors |
 * | or copyright holders shall not be liable |
 * | for any claims, damages or other         |
 * | liability, whether in contract, tort or  |
 * | otherwise, arising out of or in          |
 * | connection with the software or your use |
 * | or other dealings with the software.     |
 * |__________________________________________|
 * |   website: tolsedum.ru                   |
 * |   email: tolsedum@gmail.com              |
 * |   email: tolsedum@yandex.ru              |
 * |__________________________________________|
 */

#include <array>
#include <exception>
#include <mutex>
#include "functions.hpp"
#include "archive/Archiver.hpp"

struct FileInfo{
    std::string file_name_;
    int file_size_;
    bool in_archive_;

    FileInfo(
        std::string file_name,
        int file_size,
        bool in_archive
    )
        : file_name_(file_name)
        , file_size_(file_size)
        , in_archive_(in_archive)
    {}
};

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
        : create_file_if_not_exists_(create_file_if_not_exists)
    {};
    FileBuffer(
        std::string file_name, bool create_file_if_not_exists
    );
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
    bool putBufferMessage(
        std::string message, std::size_t buffer_size_
    );
};

enum log_level { error, warning, critical, debug };
inline static std::array<
        std::string, log_level::debug + 1u
    > log_level_str = {
    "ERROR", "WARNING", "CRITICAL", "DEBUG"
};

/**
 * @brief Logger object
 * @author Tolsedum
 */
class Logger{
private:
    std::map<std::string, FileBuffer> buffer_;
    std::map<std::string, FileInfo> file_info_;
    std::size_t buffer_size_;
    std::mutex mutex_;
    bool create_file_if_not_exists_;

    int size_file_to_zip_;
    time_t time_live_zip_files_;

    void flashToFile(
        std::string name_file, std::vector<std::string>& list
    );

    void collectFileInfo(std::string file_name, int f_size);
public:
    Logger(
        std::size_t buffer_size = 10u,
        bool create_file_if_not_exists = false,
        int size_file_to_zip = 0,
        unsigned long time_live_zip_files = 0 //(365 * 24 * 60 * 60)
    )
        : buffer_size_(buffer_size)
        , create_file_if_not_exists_(create_file_if_not_exists)
        , size_file_to_zip_(size_file_to_zip)
        , time_live_zip_files_(time_live_zip_files)
    {};
    ~Logger(){flash();};

    void log(
        const std::string message,
        const std::string name_file,
        log_level level = log_level::debug,
        bool create_file_if_not_exists = false
    );

    void flash();

    void archivingLogFiles();
};

#endif /* LOGGER_HPP */
