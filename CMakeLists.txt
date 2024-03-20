cmake_minimum_required(VERSION 3.26)
project(cdn-project)
set(CMAKE_CXX_STANDARD 14)

include(${CMAKE_CURRENT_SOURCE_DIR}/edge-server/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/client/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/origin-server/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/cdn-node/CMakeLists.txt)

