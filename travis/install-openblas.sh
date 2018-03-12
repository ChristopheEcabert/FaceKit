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
if [ -d "$INSTALL_DIR" ] && ls $INSTALL_DIR/lib/*.so >/dev/null 2>&1; then
  echo "Using cached build at $INSTALL_DIR ..."
else
  rm -rf $INSTALL_DIR
  git clone --depth 1 https://github.com/xianyi/OpenBLAS.git ${INSTALL_DIR}/source 
  cd $INSTALL_DIR/source
  # Redirect build output to a log and only show it if an error occurs
  # Otherwise there is too much output for TravisCI to display properly
  make FC="$FC" NO_AFFINITY=1 USE_THREAD=0 -j8
  make FC="$FC" PREFIX=$INSTALL_DIR install 
  # Remove source
  cd ${INSTALL_DIR} && rm -rf ${INSTALL_DIR}/source
fi
# Update path file
export PATH=$INSTALL_DIR:${PATH}
