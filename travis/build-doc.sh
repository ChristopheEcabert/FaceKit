#! /bin/bash
set -e
echo "Build Documentation"
cd $TRAVIS_BUILD_DIR
if [[ "$DOCUMENTATION" == "TRUE" ]]; then
  #echo "Already some doc ?"
  #cd build && ls modules/doc
  echo "Generate"
  cd build && make doc
  echo "Check"
  pwd
  cat modules/doc/Doxyfile
  ls modules/doc/html
fi