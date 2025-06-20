cmake_minimum_required(VERSION 3.20)
project(grpc-server LANGUAGES C CXX)

# =======================
# Dependencies
# =======================
find_package(protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(utf8_range REQUIRED)

# =======================
# Compiler Settings
# =======================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# =======================
# Protobuf / gRPC Codegen Library
# =======================
set(GENPROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/genproto")
set(PROTO_SRC
    "${GENPROTO_DIR}/order_service/order.pb.cc"
    "${GENPROTO_DIR}/order_service/order.grpc.pb.cc"
)
set(PROTO_HEADERS
    "${GENPROTO_DIR}/order_service/order.pb.h"
    "${GENPROTO_DIR}/order_service/order.grpc.pb.h"
)

add_library(genproto_lib STATIC
    ${PROTO_SRC}
    ${PROTO_HEADERS}
)

target_include_directories(genproto_lib
    PUBLIC ${GENPROTO_DIR}
)

target_link_libraries(genproto_lib
    PUBLIC
        gRPC::grpc++
        protobuf::libprotobuf
        utf8_range::utf8_range
        utf8_range::utf8_validity
        absl::log_severity
        absl::strings
        absl::core_headers
        absl::time
        absl::synchronization
        absl::utility
        absl::base
)

# =======================
# Application Executable
# =======================
set(APP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/service/order_service.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/server.cpp"
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${APP_SOURCES})

set(APP_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/common.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/server/server.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/service/order_service.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/config/config.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/interceptors/logger.hpp"
)

# Server
add_executable(${PROJECT_NAME}
    ${APP_SOURCES}
    ${APP_HEADERS}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE genproto_lib
)

# =======================
# Client executable
# ========================
set(CLIENT_NAME "grpc-client")

add_executable(${CLIENT_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/src/client.cpp"
)

target_include_directories(${CLIENT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(${CLIENT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/genproto
)

target_link_libraries(${CLIENT_NAME}
    PRIVATE genproto_lib
)

target_compile_options(${PROJECT_NAME}
    PRIVATE -Wall -Wno-deprecated
)

# =======================
# Output
# =======================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
