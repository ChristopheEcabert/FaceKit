#! /bin/bash
set -e
echo "Build Documentation"
cd $TRAVIS_BUILD_DIR
if [[ "$DOCUMENTATION" == "TRUE" ]]; then
  echo "Generate"
  cd build && make doc
fi