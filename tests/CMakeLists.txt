add_executable(test threadpool_test.cc)
target_link_libraries(test PRIVATE Catch2::Catch2WithMain)
target_include_directories(test PRIVATE ${CMAKE_SOURCE_DIR})