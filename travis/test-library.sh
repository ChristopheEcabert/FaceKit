#! /bin/bash
set -e
echo "Call Unit Test"
cd $TRAVIS_BUILD_DIR
cd build && make test
cat Testing/Temporary/LastTest.log