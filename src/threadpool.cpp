//
// Created by anuj on 10/6/25.
//

#include "threadpool.h"

ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for(size_t i=0; i<threads;++i){
        workers.emplace_back([this](){
            while(!stop){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this](){return this->stop || !this->tasks.empty();});
                    if (this->stop && this->tasks.empty()) return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        });
    }
}
void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.push(std::move(task));
    }
    condition.notify_one();
}

ThreadPool::~ThreadPool() {
    stop = true;
    condition.notify_all();
    for (std::thread &worker : workers) {
        if (worker.joinable()) worker.join();
    }
}