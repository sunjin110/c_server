cmake_minimum_required(VERSION 3.22.1)

# 補完のためのcompile_commands.jsonを生成する
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# helper
# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# CONAN_BASIC_SETUP()
include(CMakePrintHelpers)
cmake_print_variables(CONAN_LIBS)
cmake_print_variables(CMAKE_CXX_LINK_EXECUTABLE)

project(c_server)

# moldリンカ設定
if (LINUX)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=mold")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=mold")
elseif(APPLE)
  # macでのCPP buildはmoldは対応していない
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=lld")
else()
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=mold")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=mold")
endif()

find_package(jansson REQUIRED)
include_directories(${jansson_INCLUDE_DIRS})


# find_package(sqlite3 REQUIRED)
# include_directories(${sqlite3_INCLUDE_DIRS})

# find_package(stduuid REQUIRED)
# include_directories(${stduuid_INCLUDE_DIRS})

# cmake_print_variables(stduuid_LIBRARIES)

find_package(libmysqlclient REQUIRED)
include_directories(${libmysqlclient_INCLUDE_DIRS})

file(GLOB_RECURSE src ${PROJECT_SOURCE_DIR} "src/domain/*.h" "src/*.h" "src/*.c" 
  "lib/mustach/mustach.h" "lib/mustach/mustach.c" "lib/mustach/mustach-jansson.h" "lib/mustach/mustach-jansson.c"
  "lib/mustach/mustach-wrap.h" "lib/mustach/mustach-wrap.c")

add_executable(${PROJECT_NAME} main.c ${src})


if(LINUX)
  target_link_libraries(${PROJECT_NAME} ${libmysqlclient_LIBRARIES} ${jansson_LIBRARIES} ${sqlite3_LIBRARIES} uuid m)
elseif(APPLE)
  target_link_libraries(${PROJECT_NAME} ${libmysqlclient_LIBRARIES} ${jansson_LIBRARIES} ${sqlite3_LIBRARIES} m)
else(LINUX)
  target_link_libraries(${PROJECT_NAME} ${libmysqlclient_LIBRARIES} ${jansson_LIBRARIES} ${sqlite3_LIBRARIES} uuid m)
endif(LINUX)

target_compile_options(${PROJECT_NAME} PUBLIC -O2 -Wall)

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

if(LINUX)
  target_link_libraries( c_server_test gtest::gtest ${jansson_LIBRARIES} ${libmysqlclient_LIBRARIES} uuid m)
elseif(APPLE)
  target_link_libraries( c_server_test gtest::gtest ${jansson_LIBRARIES} ${libmysqlclient_LIBRARIES} m)
else(LINUX)
  target_link_libraries( c_server_test gtest::gtest ${jansson_LIBRARIES} ${libmysqlclient_LIBRARIES} uuid m)
endif(LINUX)

include(GoogleTest)
gtest_discover_tests(c_server_test)
