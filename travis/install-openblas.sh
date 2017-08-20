#! /bin/bash
# Copyright (c) 2016, NVIDIA CORPORATION.  All rights reserved.
# Taken  from https://github.com/lukeyeager/DIGITS
set -e
echo "Install OpenBLAS"
LOCAL_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if [ "$#" -ne 1 ];
then
    echo "Usage: $0 INSTALL_DIR"
    exit 1
fi

INSTALL_DIR=`readlink -f $1`
if [ -d "$INSTALL_DIR" ] && ls $INSTALL_DIR/*.so >/dev/null 2>&1; then
    echo "Using cached build at $INSTALL_DIR ..."
else
    rm -rf $INSTALL_DIR
    git clone https://github.com/xianyi/OpenBLAS.git ${INSTALL_DIR} -b v0.2.18 --depth 1
    cd $INSTALL_DIR
    # Redirect build output to a log and only show it if an error occurs
    # Otherwise there is too much output for TravisCI to display properly
    LOG_FILE=$LOCAL_DIR/openblas-build.log
    make NO_AFFINITY=1 USE_THREAD=0 -j8 >$LOG_FILE 2>&1 || (cat $LOG_FILE && false)
fi
cd $INSTALL_DIR
sudo make install PREFIX=/usr/local