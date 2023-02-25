
b:
	touch CMakeLists.txt
	cmake --build build

init:
	mkdir build
	mkdir release
	cmake -S . -B build
	cmake -S . -B release -DCMAKE_BUILD_TYPE=Release

product_build:
	touch CMakeLists.txt
	cmake --build release

run:
	make b
	./build/main

fmt:
	clang-format -i --verbose ./main.c
	find src -name "*.c" -or -name "*.h" | xargs clang-format -i --verbose
	
