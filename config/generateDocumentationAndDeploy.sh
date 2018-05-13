echo 'Setting up the script...'

set -e

mkdir code_docs
cd code_docs

git clone -b gh-pages https://github.com/${TRAVIS_REPO_SLUG}
cd ${TRAVIS_REPO_SLUG##*/}

git config --global push.default simple
git config user.name "Travis CI"
git config user.email "travis@travis-ci.org"

rm -f !(index.html)

echo "" > .nojekyll

echo 'Generating Doxygen code documentation...'

doxygen $DOXYFILE 2>&1 | tee doxygen.log

if [ -d "docs/html" ] && [ -f "docs/html/index.html" ]; then

    echo 'Uploading documentation to the gh-pages branch...'

    git add --all

    git commit -m "Deploy code docs to GitHub Pages Travis build: ${TRAVIS_BUILD_NUMBER}" -m "Commit: ${TRAVIS_COMMIT}"

   
    git push --force "https://${GH_REPO_TOKEN}@github.com/${TRAVIS_REPO_SLUG}"
else
    echo '' >&2
    echo 'Warning: No documentation (html) files have been found!' >&2
    echo 'Warning: Not going to push the documentation to GitHub!' >&2
    exit 1
fi