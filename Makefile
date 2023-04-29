b:
	make gen
	touch CMakeLists.txt
	# cmake . -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -B build
	cmake . -DCMAKE_BUILD_TYPE=Release -B build
	cmake --build build

compile_commands:
	# fleetとかで保管するのに必要なcompile_commands.jsonを生成する
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build

hard_build:
	# なんか色々上手くいかんときは、大体これをすれば上手くいく
	touch CMakeLists.txt
	make gen
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

gen:
	./generate_template_to_h.sh

run:
	make b
	./build/c_server

fmt:
	clang-format -i --verbose ./main.c
	find src -name "*.c" -or -name "*.h" | xargs clang-format -i --verbose
	
tidy:
	conan install . --output-folder=build --build=missing

docker_build:
	docker build . -t github.com/sunjin110/c_server:latest --no-cache

docker_run:
	make docker_build
	docker run -p 8088:8088 -it github.com/sunjin110/c_server:latest

docker_build_test:
	docker build . -f Dockerfile.test -t github.com/sunjin110/c_server_test:latest --no-cache

docker_test:
	make docker_build_test
	docker run -it github.com/sunjin110/c_server_test:latest

dev_up:
	docker compose -f docker-compose.dev.yml up

dev_down:
	docker compose -f docker-compose.dev.yml down 

dev_clear:
	docker system prune -y
	docker volume rm c_server_mariadb_data

