b:
	touch CMakeLists.txt
	cmake --build build

hard_build:
	# なんか色々上手くいかんときは、大体これをすれば上手くいく
	touch CMakeLists.txt
	rm -rf build
	conan install . --output-folder=build --build=missing
	cmake . -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -B build
	cmake --build build

clean:
	rm -rf build
	conan install . --output-folder=build --build=missing
	cmake . -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -B build

t:
	make b
	./build/c_server_test

# init:
# 	brew install gcc
# 	mkdir build
# 	mkdir release
# 	cmake -S . -B build
# 	cmake -S . -B release -DCMAKE_BUILD_TYPE=Release

run:
	make b
	./build/c_server

fmt:
	clang-format -i --verbose ./main.c
	find src -name "*.c" -or -name "*.h" | xargs clang-format -i --verbose
	
tidy:
	conan install . --output-folder=build --build=missing

docker_build:
	docker build . -t github.com/sunjin110/c_server:latest

docker_run:
	make docker_build
	docker run -p 8088:8088 -it github.com/sunjin110/c_server:latest

docker_build_test:
	docker build . -f Dockerfile.test -t github.com/sunjin110/c_server_test:latest

docker_test:
	make docker_build_test
	docker run -it github.com/sunjin110/c_server_test:latest


# docker run -v "/var/run/docker.sock":"/var/run/docker.sock" --entrypoint=sh github.com/sunjin110/c_server_test:latest -c "`cat semicolon_delimited_script`"