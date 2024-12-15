#include "Logger.hpp"

#include <sys/stat.h>
#include <ctime>
#include <stdio.h>

#include <thread>
#include <semaphore>


// std::atomic_flag lock_queue_atomic_flag{ATOMIC_FLAG_INIT};
// std::binary_semaphore lock_queue_sem{1};

// class Queue{
// private:
//     bool is_detected_;
//     int count;

//     unsigned long getHash(){
//         std::hash<std::thread::id> hasher;
//         return hasher(std::this_thread::get_id());
//     }

// public:
//     static unsigned long not_locked;
//     ~Queue(){
//         if(is_detected_){
//             for (int i = 0; i < count; i++){
//                 unlock();
//             }
//         }else{
//             unlock();
//         }
//     }
//     Queue(bool is_detected)
//         : is_detected_(is_detected)
//     {
//         count = 0;
//         not_locked = 0;
//     }

//     void lock(){
//         if(not_locked != getHash()){
//             if(is_detected_){
//                 count++;
//                 lock_queue_sem.acquire();
//             }else{
//                 lock_queue_sem.acquire();
//             }
//         }
//     }

//     void unlock(){
//         if(is_detected_ && not_locked == getHash()){
//             not_locked = 0;
//             count--;
//         }
//         lock_queue_sem.release();
//     }

//     bool try_acquire(){
//         bool result = false;
//         if(is_detected_){
//             if(!count){
//                 result = lock_queue_sem.try_acquire();
//                 if(result){
//                     not_locked = getHash();
//                     count++;
//                 }
//             }
//         }else{
//             result = lock_queue_sem.try_acquire();
//         }
//         return result;
//     }
// };

// unsigned long Queue::not_locked = 0;

Logger &getLog(){
    static unsigned buffer_size = 1000;
    LoggerParams l_params{buffer_size, 1024, 60*60*2, true};

    static Logger log(l_params);
    return log;
}

// Queue &getQueue(){
//     static Queue queue_(true);
//     return queue_;
// }


// void worker(int id){
//     Queue queue(true);
//     if(queue.try_acquire()){
//         std::cout << "Поток " << id << " зашёл\n";
//     }else{
//         std::cout << "Поток " << id << " не зашёл\n";
//     }

//     queue.lock();
//     std::cout << "Поток " << id << " работает\n";
//     std::this_thread::sleep_for(std::chrono::milliseconds(4000)); // Симуляция работы
//     std::cout << "Поток " << id << " закончил\n";
//     queue.unlock();

// }

// std::atomic_flag lock = ATOMIC_FLAG_INIT;

// int argc, char *argv[]

void insert(std::map<std::string, int> &test, std::string str, int i){
    auto [search, success] = test.insert({str, i});
    std::cout
        << "success: " << success
        << " first: " << search->first
        << " second: " << search->second
    << std::endl;
}

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

    // std::thread t1(worker, 1);
    // std::thread t2(worker, 2);

    // t1.join();
    // t2.join();

    std::string file_name{"log/server/request.log"};

    for (size_t i = 0; i < 1; i++){
            std::string str{"127.0.0.1:8080 Request: content type: text/html; file path: public/test.file; status: Forbidden;"};
        for (auto &&i : {
            log_level::critical,
            // log_level::debug,
            // log_level::error,
            // log_level::warning
        }){
            getLog().log(
                str,
                file_name,
                i
            );
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
