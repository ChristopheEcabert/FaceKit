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
if [ -d "$INSTALL_DIR" ] && ls $INSTALL_DIR/build/lib/*$EXT >/dev/null 2>&1; then
    echo "Using cached build at $INSTALL_DIR ..."
else
    rm -rf $INSTALL_DIR
    git clone https://github.com/opencv/opencv.git ${INSTALL_DIR} --depth 1
    cd $INSTALL_DIR
    # Redirect build output to a log and only show it if an error occurs
    # Otherwise there is too much output for TravisCI to display properly
    LOG_FILE=$LOCAL_DIR/opencv-build.log
    mkdir -p build && cd build
    cmake -DCMAKE_PREFIX_INSTALL=/usr/local -DCMAKE_BUILD_TYPE=Release -DBUILD_DOCS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF ..
    #make -j8 >$LOG_FILE 2>&1 || (cat $LOG_FILE && false)
    make -j8
fi
cd $INSTALL_DIR/build
sudo make install