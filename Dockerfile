from ubuntu:22.04

# install 
run apt-get update
run apt-get upgrade -y
run apt-get install cmake -y
run apt-get install python3-pip -y
run apt-get install git -y
run apt-get install wget -y
run apt-get install zip -y
run apt-get install build-essential -y
run pip install conan

workdir /tmp
run wget https://github.com/rui314/mold/releases/download/v1.10.1/mold-1.10.1-$(arch)-linux.tar.gz
run tar -zxvf /tmp/mold-1.10.1-$(arch)-linux.tar.gz
run cp /tmp/mold-1.10.1-$(arch)-linux/bin/mold /usr/local/bin/mold
run ln -s /usr/local/bin/mold /usr/local/bin/ld.mold

run conan profile detect

# run git clone https://gitlab.com/jobol/mustach.git


workdir /root
copy . .
workdir /root/lib
run git clone  https://gitlab.com/jobol/mustach.git
workdir /root/lib/mustach
run git checkout 1.2.5

workdir /root

run make hard_build

cmd ./build/c_server