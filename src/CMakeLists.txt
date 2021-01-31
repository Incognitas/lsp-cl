
set(TARGET_NAME lspfs)

# create the actual library
add_library(${TARGET_NAME} clangd_helper.cpp 
							${PROJECT_SOURCE_DIR}/include/lspfs/clangd_helper.h
							${PROJECT_SOURCE_DIR}/include/lspfs/json_rpc_helper.h
							${PROJECT_SOURCE_DIR}/include/lspfs/messages/didopen_notification.h
							${PROJECT_SOURCE_DIR}/include/lspfs/messages/message_parameter.hpp
							${PROJECT_SOURCE_DIR}/include/lspfs/messages/initialize_request.h
							${PROJECT_SOURCE_DIR}/include/lspfs/capabilities/client_capabilities.h
							${PROJECT_SOURCE_DIR}/include/lspfs/capabilities/text_document_capabilities.h
							${PROJECT_SOURCE_DIR}/include/lspfs/capabilities/server_capabilities.h
							${PROJECT_SOURCE_DIR}/src/json_rpc_helper.cpp
							${PROJECT_SOURCE_DIR}/src/didopen_notification.cpp
							${PROJECT_SOURCE_DIR}/src/message_parameter.cpp
							${PROJECT_SOURCE_DIR}/src/initialize_request.cpp
							${PROJECT_SOURCE_DIR}/src/client_capabilities.cpp
							${PROJECT_SOURCE_DIR}/src/server_capabilities.cpp)

# ensure we are using recent C++ just in case, we can adapt later
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
# link to appropriate packages
target_link_libraries(${TARGET_NAME} PUBLIC CONAN_PKG::boost CONAN_PKG::nlohmann_json)
