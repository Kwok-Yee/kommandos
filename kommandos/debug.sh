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

# REMOVE ALL 
rm -rf *

# CREATE .nojekyll
echo "" > .nojekyll

# CURRENT WORKING DIRECTORY
echo $PWD

# FIND ALL IN CURRENT WORKING DIRECTORY
find .

# GO BACK ONE DIRECTORY
cd ..

# CURRENT WORKING DIRECTORY
echo $PWD

# FIND ALL IN CURRENT WORKING DIRECTORY
find .

# GO BACK ONE DIRECTORY
cd ..

# CURRENT WORKING DIRECTORY
echo $PWD

# FIND ALL IN CURRENT WORKING DIRECTORY
find .