#ifndef THREADPOOLCPP_HPP_
#define THREADPOOLCPP_HPP_
#include "task_queue.hpp"

#include <atomic>
#include <functional>
#include <memory>
#include <thread>

constexpr unsigned int MAX_TASK_COUNT = 100;

class ThreadPool {

  using Task = std::function<void()>;

public:
  ThreadPool(unsigned int num_threads = std::thread::hardware_concurrency())
      : num_threads_(num_threads), task_queue_(MAX_TASK_COUNT) {
    Start(num_threads);
  }
  ~ThreadPool() { Stop(); }

  void AddTask(const Task &t) { task_queue_.Put(t); }

  void AddTask(Task &&t) { task_queue_.Put(std::forward<Task>(t)); }

  void Stop() {
    std::call_once(stop_flag_, &ThreadPool::StopThreadGroup, this);
  }

private:
  void Start(unsigned int num_threads) {
    running_ = true;
    for (size_t i = 0; i < num_threads; ++i) {
      thread_group_.push_back(
          std::make_shared<std::thread>(&ThreadPool::RunInThread, this));
    }
  }

  void RunInThread() {
    while (running_) {
      std::list<Task> buffer;
      task_queue_.Take(buffer);
      for (auto &task : buffer) {
        if (!running_) {
          return;
        }
        task();
      }
    }
  }

  void StopThreadGroup() {
    task_queue_.Stop();
    running_ = false;
    for (auto thread : thread_group_) {
      if (thread) {
        thread->join();
      }
    }
    thread_group_.clear();
  }

private:
  unsigned num_threads_;
  Queue<Task> task_queue_;
  std::list<std::shared_ptr<std::thread>> thread_group_;
  std::atomic_bool running_;
  std::once_flag stop_flag_;
};

#endif