cmake_minimum_required(VERSION 3.25.2)

# helper
include(CMakePrintHelpers)

project(c_server)

find_package(ZLIB REQUIRED)

file(GLOB_RECURSE src ${PROJECT_SOURCE_DIR} "src/*.h" "src/*.c")

add_executable(${PROJECT_NAME} main.c ${src})
target_link_libraries(${PROJECT_NAME} ZLIB::ZLIB)

# use mold
add_link_options(-fuse-ld=mold)

target_compile_options(${PROJECT_NAME} PUBLIC -O2 -Wall)

