cmake_minimum_required(VERSION 3.1)
project(mutex_test)
include_directories(../../include)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11")
FIND_PACKAGE(Threads)

add_executable(mutex_test mutextest.cpp ../../src/LMutex.cpp)
TARGET_LINK_LIBRARIES(mutex_test ${CMAKE_THREAD_LIBS_INIT})