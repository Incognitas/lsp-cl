
set(TARGET_NAME lspfs)

# create the actual library
add_library(${TARGET_NAME} clangd_helper.cpp )

# ensure we are using recent C++ just in case, we can adapt later
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include/${TARGET_NAME})

# link to appropriate packages
target_link_libraries(${TARGET_NAME} PUBLIC CONAN_PKG::boost CONAN_PKG::nlohmann_json)
