#include "Logger.hpp"

#include <sys/stat.h>
#include <ctime>
#include <stdio.h>

// int argc, char *argv[]
int main() try{
    std::string file_name{"test/test.log"};

    // std::string file_n{std::filesystem::path(file_name).stem()};
    // std::size_t pos = file_n.find_last_of("_");
    // std::string rename_file;
    // int count = 0;
    // if(pos != std::string::npos){
    //     count = ufn::strToInt(file_n.substr(pos+1));
    //     std::cout << file_n.substr(pos+1) << " " << count << std::endl;
    //     file_n.erase(pos);
    //     rename_file = file_n;
    //     rename_file.append("_").append(std::to_string(++count));
    // }
    // std::cout << rename_file << std::endl;

    Logger log(10, true, 100, 60*2);

    log.log("save data1", file_name, log_level::error, true);
    log.log("save data2", file_name, log_level::critical);
    log.log("save data3", file_name, log_level::warning);
    log.log("save data4", file_name);
    log.log("save data5", file_name, log_level::error, true);
    log.log("save data6", file_name, log_level::critical);
    log.log("save data7", file_name, log_level::warning);
    log.log("save data8", file_name);
    log.log("save data9", file_name);
    log.log("save data10", file_name);
    log.log("save data11", file_name, log_level::error, true);
    log.log("save data12", file_name, log_level::critical);
    log.log("save data13", file_name, log_level::warning);

    std::cout << "Hello world!" << std::endl;
    return EXIT_SUCCESS;
}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
}
