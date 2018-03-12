#! /bin/bash
set -e
echo "Call Unit Test"
cd $TRAVIS_BUILD_DIR
cd build && make test
# Test output are only shown if failure happened