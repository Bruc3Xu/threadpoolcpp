#ifndef TASK_QUEUE_HPP_
#define TASK_QUEUE_HPP_

#include <condition_variable>
#include <list>
#include <mutex>

template <typename T> class Queue {
public:
  Queue(unsigned int size) : max_size_(size) {}

  void Put(const T &item) { Add(item); }

  void Put(T &&item) { Add(std::forward<T>(item)); }

  void Take(std::list<T> &buffer) {
    std::unique_lock<std::mutex> lock(mutex_);
    not_empty_.wait(lock, [this]() { return ask_stop_ || NotEmpty(); });

    if (ask_stop_) {
      return;
    }
    buffer = std::move(buffer_);
    not_full_.notify_one();
  }

  void Stop() {
    {
      std::lock_guard<std::mutex> lock(mutex_);
      ask_stop_ = true;
    }
    not_empty_.notify_all();
    not_full_.notify_all();
  }

  bool Full() {
    std::lock_guard<std::mutex> lock(mutex_);
    return buffer_.size() == max_size_;
  }

  bool Empty() {
    std::lock_guard<std::mutex> lock(mutex_);
    return buffer_.empty();
  }

private:
  bool NotFull() { return buffer_.size() < max_size_; }

  bool NotEmpty() { return buffer_.size() > 0; }

  template <typename F> void Add(F &&item) {
    std::unique_lock<std::mutex> lock(mutex_);
    not_full_.wait(lock, [this]() { return ask_stop_ || NotFull(); });
    if (ask_stop_) {
      return;
    }
    buffer_.push_back(std::forward<F>(item));
    not_empty_.notify_one();
  }

private:
  std::list<T> buffer_;
  unsigned int max_size_;
  std::mutex mutex_;
  std::condition_variable not_empty_;
  std::condition_variable not_full_;
  bool ask_stop_ = false;
};

#endif