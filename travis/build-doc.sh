#! /bin/bash
set -e
echo "Build Documentation"
cd $TRAVIS_BUILD_DIR
if [[ "$DOCUMENTATION" == "TRUE" ]]; then
  cd build && make doc
  cat modules/doc/Doxyfile
  ls modules/doc/html
fi