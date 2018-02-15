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
if [ -d "$INSTALL_DIR" ] && ls $INSTALL_DIR/bin/doxygen >/dev/null 2>&1; then
    echo "Using cached build at $INSTALL_DIR ..."
else
    rm -rf $INSTALL_DIR
    git clone https://github.com/doxygen/doxygen.git ${INSTALL_DIR}/source --depth 1
    cd $INSTALL_DIR/source
    # Redirect build output to a log and only show it if an error occurs
    # Otherwise there is too much output for TravisCI to display properly
    mkdir -p build && cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} ..
    make -j8
    make install
    # Remove source
    cd ${INSTALL_DIR} && rm -rf ${INSTALL_DIR}/source
fi
# Update path file
export PATH=$INSTALL_DIR:${PATH}