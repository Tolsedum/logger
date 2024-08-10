#include "Logger.hpp"

#include <sys/stat.h>
#include <ctime>
#include <stdio.h>

Logger &getLog(){
    static int buffer_size = 10;
    static Logger log(buffer_size,true, 1000, 60*2);
    return log;
}

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

    // Logger log(10, true, 100, 60*2);

    getLog().log("save data1", file_name, log_level::error, true);
    getLog().log("save data2", file_name, log_level::critical);
    getLog().log("save data3", file_name, log_level::warning);
    getLog().log("save data4", file_name);
    getLog().log("save data5", file_name, log_level::error, true);
    getLog().log("save data6", file_name, log_level::critical);
    getLog().log("save data7", file_name, log_level::warning);
    getLog().log("save data8", file_name);
    getLog().log("save data9", file_name);
    getLog().log("save data10", file_name);
    getLog().log("save data11", file_name, log_level::error, true);
    getLog().log("save data12", file_name, log_level::critical);
    getLog().log("save data13", file_name, log_level::warning);
    getLog().log("save data10", file_name);
    getLog().log("save data11", file_name, log_level::error, true);
    getLog().log("save data12", file_name, log_level::critical);
    getLog().log("save data13", file_name, log_level::warning);
    getLog().log("save data1", file_name, log_level::error, true);
    getLog().log("save data2", file_name, log_level::critical);
    getLog().log("save data3", file_name, log_level::warning);
    getLog().log("save data4", file_name);
    getLog().log("save data5", file_name, log_level::error, true);
    getLog().log("save data6", file_name, log_level::critical);
    getLog().log("save data7", file_name, log_level::warning);
    getLog().log("save data8", file_name);
    getLog().log("save data9", file_name);

    // std::string f_name = "test/test.txt";
    // std::string parent_path = ufn::getParentDir(file_name);
    // int count = 0;
    // for (auto &&path_ :
    //     std::filesystem::directory_iterator(
    //         parent_path
    // )){
    //     std::string rename_file{path_.path().parent_path()};
    //     std::string file_n = path_.path().stem();
    //     std::size_t pos = file_n.find_last_of("_");
    //     if(pos != std::string::npos && path_.path().extension() == ".zip"){
    //         count++;
    //     }
    //     std::cout
    //         << "pos: " << pos
    //         << " file_n: " << file_n
    //         << " rename_file: " << rename_file
    //         << " count: " << count
    //     <<std::endl;
    // }

    // for (int i = count; i > 0; i--){
    //     std::cout << i <<std::endl;
    // }

    std::cout << "Hello world!" << std::endl;
    return EXIT_SUCCESS;
}catch(const std::runtime_error& e){
    std::cerr << e.what() << '\n';
}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
}
