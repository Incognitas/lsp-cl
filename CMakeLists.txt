cmake_minimum_required(VERSION 3.10)

# set the project name
project(lsp_sandbox)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TARGET_NAME lsp_ref)

add_library(${TARGET_NAME} clangd_helper.cpp )
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)

target_link_libraries(${TARGET_NAME} PUBLIC CONAN_PKG::boost CONAN_PKG::nlohmann_json)

# create the main executable that will be used for testing
set(TARGET_NAME sandbox)

add_executable(${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME} PRIVATE $<TARGET_FILE:lsp_ref> CONAN_PKG::boost)

