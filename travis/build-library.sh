#! /bin/bash
set -e
echo "Build Library"
# Update path for finding deps
export PATH=${OPENBLAS_ROOT}:${OPENCV_ROOT}:${DOXYGEN_ROOT}/bin:${PROTOBUF_ROOT}:${PATH}
cd $TRAVIS_BUILD_DIR
echo "Current path: ${PATH}"
ls ${OPENBLAS_ROOT}/lib
mkdir -p build && cd build && cmake ${CMAKE_OPTIONS} -DProtobuf_DEBUG=ON .. && make -j8 
