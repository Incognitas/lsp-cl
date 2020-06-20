cmake_minimum_required(VERSION 3.10)

# set the project name
project(lsp_sandbox)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TARGET_NAME sandbox)

# create the main executable that will be used for testing
add_executable(${TARGET_NAME} main.cpp
clangd_helper.cpp
)

set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)

target_link_libraries(${TARGET_NAME} PRIVATE CONAN_PKG::boost CONAN_PKG::nlohmann_json)

