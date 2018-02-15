#! /bin/bash
# Copyright (c) 2016, NVIDIA CORPORATION.  All rights reserved.
# Taken  from https://github.com/lukeyeager/DIGITS
set -e
echo "Install Google Protocol Buffer"
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
    git clone https://github.com/google/protobuf.git ${INSTALL_DIR}/source --depth 1
    cd $INSTALL_DIR/source
    if [ "$CXX" == "clang++" ] ; then
      COMPILER_FLAGS="-stdlib=libc++"
      LINKER_FLAGS="-stdlib=libc++"
    fi
    # Generate
    ./autogen.sh
    # Config
    ./configure CXXFLAGS=${COMPILER_FLAGS} LDFLAGS=${LINKER_FLAGS} --prefix=/${INSTALL_DIR}
    make -j8
    make install
    # Remove source, not needed anymore
    cd ${INSTALL_DIR} && rm -rf ${INSTALL_DIR}/source
fi
# Update path file
export PATH=$INSTALL_DIR:${PATH}