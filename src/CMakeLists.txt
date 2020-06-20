
set(TARGET_NAME lspfs)

# create the actual library
add_library(${TARGET_NAME} clangd_helper.cpp 
							${PROJECT_SOURCE_DIR}/include/lspfs/clangd_helper.h
							${PROJECT_SOURCE_DIR}/include/lspfs/json_rpc_helper.hpp
							${PROJECT_SOURCE_DIR}/include/lspfs/messages/didopen_json_rpc.h
							${PROJECT_SOURCE_DIR}/include/lspfs/messages/didopen_json_rpc.cpp)

# ensure we are using recent C++ just in case, we can adapt later
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
# link to appropriate packages
target_link_libraries(${TARGET_NAME} PUBLIC CONAN_PKG::boost CONAN_PKG::nlohmann_json)
