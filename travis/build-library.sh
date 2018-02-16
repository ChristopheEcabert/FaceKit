#! /bin/bash
set -e
echo "Build Library"
# Update path for finding deps
export PATH=${OPENBLAS_ROOT}:${OPENCV_ROOT}:${DOXYGEN_ROOT}/bin:${PROTOBUF_ROOT}/bin:${PROTOBUF_ROOT}:${PATH}
cd $TRAVIS_BUILD_DIR
echo "Current path: ${PATH}"
protoc --version
mkdir -p build && cd build && cmake ${CMAKE_OPTIONS} -DProtobuf_DEBUG=ON -DProtobuf_INCLUDE_DIR=${PROTOBUF_ROOT}/include .. && make -j8 
