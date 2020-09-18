# create the main executable that will be used for testing
set(TARGET_NAME sandbox)


add_executable(${TARGET_NAME} main.cpp)

set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)

target_link_libraries(${TARGET_NAME} PRIVATE lspfs CONAN_PKG::boost)

# Ouh nice we will parse our own project !! Inception time
target_compile_definitions(${TARGET_NAME} PRIVATE "WORKINGDIRECTORY=\"${PROJECT_SOURCE_DIR}/build\"")
