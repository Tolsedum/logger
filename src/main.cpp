#include "Logger.hpp"

int main(){ // int argc, char *argv[]
    Logger log(10, 1);
    log.log("save data", "test/test.log", true);

    std::cout << "Hello world!" << std::endl;
}
