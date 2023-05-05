#!/bin/sh

MUSTACHE_VERSION="1.2.5"
echo "========== install mustache ============"
rm -rf lib
cd lib
git clone https://gitlab.com/jobol/mustach.git
cd mustache
git checkout ${MUSTACHE_VERSION}

# for mac
xcode-select --install