cmake_minimum_required(VERSION 3.15)

project(onixs_nats_pub)

include_directories(/usr/local/include)

find_library(NATS_LIB NAMES nats PATHS /usr/local/lib/)
find_library(ACE_LIB NAMES ACE PATHS /usr/local/lib/OnixS)
find_library(CME_MDH_LIB NAMES onixs-cmemdh PATHS /usr/local/lib/OnixS)
find_library(CME_FIX_LIB NAMES onixs-fix-engine PATHS /usr/local/lib/OnixS)

set(CMAKE_CXX_STANDARD 11)

add_executable(onixs_nats_pub connection.h publisher.h main.cpp)
target_link_libraries(onixs_nats_pub ${NATS_LIB})