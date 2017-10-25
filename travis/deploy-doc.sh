#! /bin/bash
# See: https://gist.github.com/domenic/ec8b0fc8ab45f39403dd
#      https://github.com/boostorg/hana/blob/master/.travis.yml
set -e
echo "Deploy - Documentation"
if [ -d "$TRAVIS_BUILD_DIR/build/modules/doc/html" ]; then
  cd $TRAVIS_BUILD_DIR/build
  if [[ "$TRAVIS_PULL_REQUEST" == "false" && "$TRAVIS_BRANCH" == "develop" && "$TRAVIS_REPO_SLUG" == "ChristopheEcabert/FaceKit" ]]; then
    echo "Get exisitin documentation"
    # Suppress output to avoid leaking the token when the command fails
    REPO=`git config remote.origin.url`
    SSH_REPO=${REPO/https:\/\/github.com\//git@github.com:}
    SHA=`git rev-parse --verify HEAD`
    
    # Clone existing gh-pages
    git clone $REPO --quiet --depth 1 --branch=gh-pages doc/html &>/dev/null
    
    # Copy new doc
    #rm -rf doc/html/{search,*.png,*.css,*.js,*.html}
    cp -R modules/doc/html/* doc/html/
    ls modules/doc/html
    
    # Commit
    pushd doc/html
    git add --all .
    git commit --allow-empty -m "Update documentation to ${TRAVIS_COMMIT:0:7}"
    git push --force ${SSH_REPO} gh-pages &>/dev/null
    popd
  fi
fi