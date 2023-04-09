#include "thread_pool.h"
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

namespace spx {

ThreadPool::ThreadPool() {
  int num_threads = std::thread::hardware_concurrency();

  for (int i = 0; i < num_threads; i++) {
    threads_.emplace_back([this]() {
      while (!done_.load()) {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lock(tasks_mutex_);
          tasks_cv_.wait(lock,
                         [this]() { return done_.load() || !tasks_.empty(); });
          if (done_.load()) {
            return;
          }

          task = std::move(tasks_.front());
          tasks_.pop();
        }

        task();
      }
    });
  }
}

ThreadPool::~ThreadPool() { stop(); }

void ThreadPool::stop() {
  done_.store(true);
  tasks_cv_.notify_all();
  for (auto &thread : threads_) {
    thread.join();
  }
}

void ThreadPool::submit(std::function<void()> &&task) {
  {
    std::lock_guard<std::mutex> lock(tasks_mutex_);
    tasks_.push(std::move(task));
  }
  tasks_cv_.notify_one();
}

} // namespace spx
