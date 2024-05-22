#include "Logger.hpp"

// int argc, char *argv[]
int main() try{
    Logger log(10, 1);
    log.log("save data", "test/test.log", log_level::error, true);
    log.log("save data", "test/test.log", log_level::critical);
    log.log("save data", "test/test.log", log_level::warning);
    log.log("save data", "test/test.log");
    std::cout << "Hello world!" << std::endl;
    return EXIT_SUCCESS;
}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
}
