#include "Logger.hpp"

#include <sys/stat.h>
#include <ctime>
#include <stdio.h>

Logger &getLog(){
    static unsigned buffer_size = 10;
    LoggerParams l_params{buffer_size, 1024, 60*2, true};

    static Logger log(l_params);
    return log;
}

// std::atomic_flag lock = ATOMIC_FLAG_INIT;

// int argc, char *argv[]
int main() try{
    // std::cout
    //     << " test: " <<lock.test(std::memory_order_relaxed)
    //     << " test_and_set: " << lock.test_and_set(std::memory_order_acquire)
    //     << " test: " <<lock.test(std::memory_order_relaxed)
    // << std::endl;

    // lock.clear(std::memory_order_release);

    // std::cout
    //     // << "test_and_set: " << lock.test_and_set(std::memory_order_acquire)
    //     << " test: " <<lock.test(std::memory_order_relaxed)
    // << std::endl;

    std::string file_name{"log/server/request.log"};

    for (size_t i = 0; i < 50; i++){
            std::string str{"127.0.0.1:8080 Request: content type: text/html; file path: public/test.file; status: Forbidden;"};
        for (auto &&i : {
            log_level::critical,
            log_level::debug,
            log_level::error,
            log_level::warning
        }){
            getLog().log(
                str,
                file_name,
                i
            );
            // getLog().flash();
        }
    }

    std::cout << "Hello world!" << std::endl;
    return EXIT_SUCCESS;
}catch(const char* &e){
    std::cerr << e << '\n';
}catch(const std::string& e){
    std::cerr << e << '\n';
}catch(const std::runtime_error& e){
    std::cerr << e.what() << '\n';
}catch(const std::exception& e){
    std::cerr << e.what() << '\n';
}
