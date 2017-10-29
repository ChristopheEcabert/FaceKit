#! /bin/bash
# Copyright (c) 2016, NVIDIA CORPORATION.  All rights reserved.
# Taken  from https://github.com/lukeyeager/DIGITS
set -e
echo "Install OpenCV"
LOCAL_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if [ "$#" -ne 1 ];
then
    echo "Usage: $0 INSTALL_DIR"
    exit 1
fi

case "$OSTYPE" in
  darwin*)  
    echo "OSX" 
    EXT=".dylib"
    ;; 
  linux*)   
    echo "LINUX"
    EXT=".so"
    ;;
  msys*)    
    echo "WINDOWS"
    EXT=".lib"
    ;;
  *)        
    echo "unknown: $OSTYPE" 
    exit 1
    ;;
esac

#INSTALL_DIR=`readlink -f $1`
INSTALL_DIR=$1
if [ -d "$INSTALL_DIR" ] && ls $INSTALL_DIR/lib/*$EXT >/dev/null 2>&1; then
    echo "Using cached build at $INSTALL_DIR ..."
else
    rm -rf $INSTALL_DIR
    git clone https://github.com/opencv/opencv.git ${INSTALL_DIR}/source --depth 1
    cd $INSTALL_DIR/source
    # Update path in order to find BLAS/LAPACK
    export PATH=${OPENBLAS_ROOT}:${PATH}
    # Redirect build output to a log and only show it if an error occurs
    # Otherwise there is too much output for TravisCI to display properly
    mkdir -p build && cd build
    cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DCMAKE_BUILD_TYPE=Release -DBUILD_DOCS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF ..
    #make -j8 >$LOG_FILE 2>&1 || (cat $LOG_FILE && false)
    make -j8
    make install
    # Remove source, not needed anymore
    cd ${INSTALL_DIR} && rm -rf ${INSTALL_DIR}/source
fi
# Update path file
export PATH=$INSTALL_DIR:${PATH}