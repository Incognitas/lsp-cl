cmake_minimum_required(VERSION 3.10)

### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# set the project name
project(lsp_sandbox)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# add library sub directory
add_subdirectory(src)

# add apps used to test this library if any
add_subdirectory(apps)


