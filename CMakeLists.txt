cmake_minimum_required(VERSION 3.25.2)

# helper
include(CMakePrintHelpers)

project(c_server)

file(GLOB_RECURSE src ${PROJECT_SOURCE_DIR} "src/*.h" "src/*.c")

add_executable(main main.c ${src})

# use mold
add_link_options(-fuse-ld=mold)

target_compile_options(main PUBLIC -O2 -Wall)

