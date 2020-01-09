cmake_minimum_required(VERSION 3.1)

project(onixs_nats_pub)

include_directories(/usr/local/include)

find_library(NATS_LIB NAMES nats PATHS /usr/local/lib/)
find_library(ACE_LIB NAMES ACE PATHS /usr/local/lib/OnixS)
find_library(CME_MDH_LIB NAMES onixs-cmemdh PATHS /usr/local/lib/OnixS)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DLICENSE_FOLDER="${CMAKE_SOURCE_DIR}/../license")
add_definitions(-DCONNECTIVITY_CONFIGURATION_FILE="${CMAKE_SOURCE_DIR}/resources/config.xml")

add_subdirectory(src)
