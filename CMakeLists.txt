cmake_minimum_required(VERSION 3.1)
project(OSDesign)
include_directories(include)
link_directories(lib)

set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++17")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 
add_executable(main src/main.cpp)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)
target_link_libraries(main ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(main libLThreads.a)