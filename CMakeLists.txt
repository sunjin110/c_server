cmake_minimum_required(VERSION 3.22.1)

# helper
# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# CONAN_BASIC_SETUP()
include(CMakePrintHelpers)
cmake_print_variables(CONAN_LIBS)
cmake_print_variables(CMAKE_CXX_LINK_EXECUTABLE)

# add_custom_command()

project(c_server)

find_package(ZLIB REQUIRED)

file(GLOB_RECURSE src ${PROJECT_SOURCE_DIR} "src/*.h" "src/*.c")

add_executable(${PROJECT_NAME} main.c ${src})
target_link_libraries(${PROJECT_NAME} ZLIB::ZLIB)

# use mold
# add_link_options(-fuse-ld=mold)

# macではmoldはなんかsoldにしないといけないみたい
# add_link_options(-fuse-ld=lld)

# target_compile_options(${PROJECT_NAME} PUBLIC -Wall)
target_compile_options(${PROJECT_NAME} PUBLIC -O2 -Wall)
# target_compile_options(${PROJECT_NAME} PUBLIC -O2 -Wall -fuse-ld=mold)


## for tests
enable_testing()
# gtest: setup manual
# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

# gtest: setup conan
find_package(GTest)

# target file
file(GLOB_RECURSE test ${PROJECT_SOURCE_DIR} "test/*.cpp")

# gtest: Create and run a binary
add_executable(
    c_server_test ${src} ${test}
)

# find_package(GTest)
target_link_libraries(
  c_server_test
  gtest::gtest
)

include(GoogleTest)
gtest_discover_tests(c_server_test)
