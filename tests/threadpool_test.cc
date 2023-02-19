#include "src/threadpoolcpp.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

void TestThreadPool() {
  ThreadPool pool;

  std::thread thread1([&pool]() {
    for (size_t i = 0; i < 10; ++i) {
      pool.AddTask([i]() {
        UNSCOPED_INFO("thread " << std::this_thread::get_id() << " add task "
                                << i);
      });
    }
  });

  std::thread thread2([&pool]() {
    for (size_t i = 0; i < 10; ++i) {
      pool.AddTask([i]() {
        UNSCOPED_INFO("thread " << std::this_thread::get_id() << " add task "
                                << i);
      });
    }
  });

  std::this_thread::sleep_for(std::chrono::seconds(2));

  getchar();

  pool.Stop();

  thread1.join();
  thread2.join();
}

TEST_CASE("threadpool test", "[threadpool]") {
  TestThreadPool();
  CHECK(false);
}