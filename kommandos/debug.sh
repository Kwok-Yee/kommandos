#!/bin/bash
echo 'Setting up the script...'

set -e

mkdir code_docs
cd code_docs

git clone -b gh-pages https://github.com/${TRAVIS_REPO_SLUG}
cd ${TRAVIS_REPO_SLUG##*/}

git config --global push.default simple
git config user.name "Travis CI"
git config user.email "travis@travis-ci.org"

echo "REMOVE ALL"
rm -rf *

echo "CREATE .nojekyll"
echo "" > .nojekyll

echo "CURRENT WORKING DIRECTORY"
echo $PWD

echo "FIND ALL IN CURRENT WORKING DIRECTORY"
find .

echo "GO BACK ONE DIRECTORY"
cd ..

echo "CURRENT WORKING DIRECTORY"
echo $PWD

echo "FIND ALL IN CURRENT WORKING DIRECTORY"
find .

echo "GO BACK ONE DIRECTORY"
cd ..

echo "CURRENT WORKING DIRECTORY"
echo $PWD

echo "FIND ALL IN CURRENT WORKING DIRECTORY"
find .