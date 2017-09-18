#! /bin/bash
# Copyright (c) 2016, NVIDIA CORPORATION.  All rights reserved.
# Taken  from https://github.com/lukeyeager/DIGITS
set -e
echo "Install Doxygen"
LOCAL_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if [ "$#" -ne 1 ];
then
    echo "Usage: $0 INSTALL_DIR"
    exit 1
fi

INSTALL_DIR=`readlink -f $1`
if [ -d "$INSTALL_DIR" ] && ls $INSTALL_DIR/build/lib/*.a >/dev/null 2>&1; then
    echo "Using cached build at $INSTALL_DIR ..."
else
    rm -rf $INSTALL_DIR
    git clone https://github.com/doxygen/doxygen.git ${INSTALL_DIR} --depth 1
    cd $INSTALL_DIR
    # Redirect build output to a log and only show it if an error occurs
    # Otherwise there is too much output for TravisCI to display properly
    LOG_FILE=$LOCAL_DIR/doxygen-build.log
    mkdir -p build && cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make -j8
fi
cd $INSTALL_DIR/build
sudo make install