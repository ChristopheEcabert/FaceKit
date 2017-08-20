#! /bin/bash
set -e
echo "Build Library"
cd $TRAVIS_BUILD_DIR
mkdir -p build && cd build && cmake ${CMAKE_OPTIONS} .. && make -j8 