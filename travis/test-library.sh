#! /bin/bash
echo "Call Unit Test"
cd $TRAVIS_BUILD_DIR
cd build
if make test; then
  exit 0
fi
exit -1
# Test output are only shown if failure happened