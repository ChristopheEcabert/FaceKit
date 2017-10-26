#! /bin/bash
set -e
echo "Build Library"
# Update path for finding deps
export PATH=${OPENBLAS_ROOT}:${OPENCV_ROOT}:${DOXYGEN_ROOT}/bin:${PATH}
cd $TRAVIS_BUILD_DIR
echo ${PATH}
mkdir -p build && cd build && cmake ${CMAKE_OPTIONS} .. && make -j8 