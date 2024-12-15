#ifndef LOGGER_HPP
#define LOGGER_HPP
/** Version 2.2 */

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
#include <thread>
#include <semaphore>
#include "functions.hpp"
#include "archive/Archiver.hpp"

inline std::binary_semaphore lock_queue_sem{1};

struct FileInfo{
    std::string file_name_;
    unsigned file_size_;
    bool in_archive_;

    FileInfo(
        std::string file_name,
        unsigned long file_size,
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
    FileBuffer(){
        create_file_if_not_exists_ = false;
        file_name_ = "";
    };
    ~FileBuffer();

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
        std::string message, unsigned buffer_size
    );

    void merge(FileBuffer &&fb, unsigned buffer_size);
};

enum log_level { error, warning, critical, debug };
inline static std::array<
        std::string, log_level::debug + 1u
    > log_level_str = {
    "ERROR", "WARNING", "CRITICAL", "DEBUG"
};

struct LoggerParams{
public:
    unsigned buffer_size_;
    unsigned size_file_to_zip_;
    unsigned long time_live_zip_files_;
    bool create_file_if_not_exists_;

    LoggerParams(){
        create_file_if_not_exists_ = false;
        buffer_size_ = 10u;
        size_file_to_zip_ = 0;
        time_live_zip_files_ = 0;
    };
    LoggerParams(
        unsigned buffer_size = 10U,
        unsigned size_file_to_zip = 0,
        unsigned long time_live_zip_files = 0,
        bool create_file_if_not_exists = false

    )
        : buffer_size_(buffer_size)
        , size_file_to_zip_(size_file_to_zip)
        , time_live_zip_files_(time_live_zip_files)
        , create_file_if_not_exists_(create_file_if_not_exists)
    {};

    friend std::ostream &operator <<(
        std::ostream &o_stream, const LoggerParams &lparams
    ){
        o_stream
            << " create_file_if_not_exists_: " << lparams.create_file_if_not_exists_
            << " buffer_size: " << lparams.buffer_size_
            << " size_file_to_zip: " << lparams.size_file_to_zip_
            << " time_live_zip_files: " << lparams.time_live_zip_files_
        << std::endl;
        return o_stream;
    }
};

/**
 * @brief Logger object
 * @author Tolsedum
 */
class Logger{
private:
    std::map<std::string, FileBuffer> buffer_;
    std::map<std::string, FileInfo> file_info_;
    unsigned buffer_size_;
    bool create_file_if_not_exists_;


    unsigned long size_file_to_zip_;
    unsigned long time_live_zip_files_;

    void flashToFile(
        std::string name_file, std::vector<std::string>&& list
    );

    void collectFileInfo(
        std::string file_name, unsigned long f_size);

    int getMaxNum(const std::string &name);

    void deleteFileIfTimesUp(
        const std::string &dir_path,
        const std::string &template_name
    );

    std::string getNameFileForArchive(
        std::string &parent_path, std::string &template_file_name
    );

    void insertInLog(
        const std::string message,
        const std::string name_file,
        log_level level,
        bool create_file_if_not_exists

    );
public:
    /**
     * @brief Loger construct
     * @param buffer_size The number of lines stored in the buffer. If there are more lines, they are transferred to the log file
     * @param create_file_if_not_exists create file if not exists
     * @param size_file_to_zip file size for archiving
     * @param time_live_zip_files The period after which the archive will be deleted
     * @param begin_time_period the beginning of the period of deleting and archiving files
     */
    Logger(
        unsigned buffer_size = 10u,
        bool create_file_if_not_exists = false,
        unsigned size_file_to_zip = 0,
        unsigned long time_live_zip_files = 0 //(365 * 24 * 60 * 60)
    )
        : buffer_size_(buffer_size)
        , create_file_if_not_exists_(create_file_if_not_exists)
        , size_file_to_zip_(size_file_to_zip)
        , time_live_zip_files_(time_live_zip_files)
    {};
    Logger(LoggerParams loggerParams)
        : buffer_size_(loggerParams.buffer_size_)
        , create_file_if_not_exists_(loggerParams.create_file_if_not_exists_)
        , size_file_to_zip_(loggerParams.size_file_to_zip_)
        , time_live_zip_files_(loggerParams.time_live_zip_files_)
    {};
    ~Logger(){flash();};

    void log(
        const std::string message,
        const std::string name_file,
        log_level level,
        bool create_file_if_not_exists
    );
    void log(
        const std::string message,
        const std::string name_file,
        log_level level
    );

    void flash();

    void archivingLogFiles();
};

#endif /* LOGGER_HPP */
