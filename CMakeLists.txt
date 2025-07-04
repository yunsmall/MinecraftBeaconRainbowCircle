cmake_minimum_required(VERSION 3.31)
project(minecraft_beacon_circle)

set(CMAKE_CXX_STANDARD 23)

find_package(TBB CONFIG REQUIRED)

include_directories(include)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(minecraft_beacon_circle ${SRC_FILES})
target_link_libraries(minecraft_beacon_circle PRIVATE TBB::tbb)