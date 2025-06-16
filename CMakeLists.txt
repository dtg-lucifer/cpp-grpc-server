cmake_minimum_required(VERSION 3.20)
project(grpc-demo LANGUAGES C CXX)

# =======================
# Dependencies
# =======================
find_package(Protobuf REQUIRED)
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
file(GLOB_RECURSE PROTO_SRC "${GENPROTO_DIR}/*.pb.cc")
file(GLOB_RECURSE PROTO_HEADERS "${GENPROTO_DIR}/*.pb.h")

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
)

# =======================
# Application Executable
# =======================
file(GLOB_RECURSE APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE APP_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

add_executable(${PROJECT_NAME}
    ${APP_SOURCES}
    ${APP_HEADERS}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE genproto_lib
)

target_compile_options(${PROJECT_NAME}
    PRIVATE -Wall -Wextra -Wpedantic
)

# =======================
# Output
# =======================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
