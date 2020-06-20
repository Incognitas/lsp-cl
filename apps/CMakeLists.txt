# create the main executable that will be used for testing
set(TARGET_NAME sandbox)

add_executable(${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME} PRIVATE lspfs CONAN_PKG::boost)
