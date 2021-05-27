project(rendering-examples)
cmake_minimum_required(VERSION 3.11)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

set(GLTF_MODEL_PATH "${CMAKE_SOURCE_DIR}/resources/glTF-models/")
set(IBL_PATH "${CMAKE_SOURCE_DIR}/resources/images/IBL/")

include(${CMAKE_SOURCE_DIR}/cmake/macros/LinkPersonalLibraries.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/macros/GenerateModelPaths.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/macros/GenerateIBLPaths.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/macros/GeneratePlatformMacros.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/macros/FindExternalLibraries.cmake)

generate_platform_macros()
find_external_libraries()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED_ON)
if (UNIX)
  add_compile_options(-W -Wall -Wextra -Wpedantic -fvisibility=hidden -std=c++20)
endif()

set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/engine)
add_subdirectory(${CMAKE_SOURCE_DIR}/engine)
add_subdirectory(${CMAKE_SOURCE_DIR}/examples)
