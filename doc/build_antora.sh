#!/bin/bash

#
# Copyright (c) 2003 Boost.Test contributors
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/boostorg/test
#

set -ex

if [ $# -eq 0 ]
  then
    echo "No playbook supplied, using default playbook"
    PLAYBOOK="local-playbook.yml"
  else
    PLAYBOOK=$1
fi

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd "$SCRIPT_DIR"

if [ -z "${BOOST_SRC_DIR:-}" ]; then
  CANDIDATE=$( cd "$SCRIPT_DIR/../../.." 2>/dev/null && pwd )
  if [ -n "$CANDIDATE" ]; then
    BOOST_SRC_DIR_IS_VALID=ON
    for F in "CMakeLists.txt" "Jamroot" "boost-build.jam" "bootstrap.sh" "libs"; do
      if [ ! -e "$CANDIDATE/$F" ]; then
        BOOST_SRC_DIR_IS_VALID=OFF
        break
      fi
    done
    if [ "$BOOST_SRC_DIR_IS_VALID" = "ON" ]; then
      export BOOST_SRC_DIR="$CANDIDATE"
      echo "Using BOOST_SRC_DIR=$BOOST_SRC_DIR"
    fi
  fi
fi

BRANCH=master

if [ -n "${BOOST_SRC_DIR:-}" ]; then
  if [ -n "${CIRCLE_REPOSITORY_URL:-}" ]; then
    if [[ "$CIRCLE_REPOSITORY_URL" =~ boostorg/boost(\.git)?$ ]]; then
      LIB="$(basename "$(dirname "$SCRIPT_DIR")")"
      REPOSITORY="boostorg/${LIB}"
      BRANCH=$(git -C "$BOOST_SRC_DIR" rev-parse --abbrev-ref HEAD)
    else
      ACCOUNT="${CIRCLE_REPOSITORY_URL#*:}"
      ACCOUNT="${ACCOUNT%%/*}"
      LIB=$(basename "$(git rev-parse --show-toplevel)")
      REPOSITORY="${ACCOUNT}/${LIB}"
    fi
    SHA=$(git -C "$BOOST_SRC_DIR/libs" ls-tree HEAD | grep -w test | awk '{print $3}')
  elif [ -n "${GITHUB_REPOSITORY:-}" ]; then
    REPOSITORY="${GITHUB_REPOSITORY}"
    SHA="${GITHUB_SHA}"
  fi
fi

cd "$SCRIPT_DIR"

if [ -n "${REPOSITORY}" ] && [ -n "${SHA}" ]; then
  BASE_URL="https://github.com/${REPOSITORY}/blob/${SHA}"
  echo "Setting base-url to $BASE_URL"
  if [ -f mrdocs.yml ]; then
    cp mrdocs.yml mrdocs.yml.bak
    perl -i -pe 's{^\s*base-url:.*$}{base-url: '"$BASE_URL/"'}' mrdocs.yml
  fi
else
  echo "REPOSITORY or SHA not set; skipping base-url modification"
fi

# Antora's git backend requires <content-source>/.git to be a directory. Inside
# a Boost superproject checkout libs/test is a submodule, so its .git is a file
# and Antora refuses the source. Derive an equivalent playbook rooted at the
# superproject in that case. A standalone clone (which is what CI checks out)
# takes neither branch and uses the playbook as written.
if [ -f "$SCRIPT_DIR/../.git" ]; then
  if [ -n "${BOOST_SRC_DIR:-}" ] && [ -d "$BOOST_SRC_DIR/.git" ]; then
    DERIVED_PLAYBOOK=".superproject-playbook.yml"
    REL_START_PATH="${SCRIPT_DIR#"$BOOST_SRC_DIR"/}"
    echo "libs/test is a submodule; deriving $DERIVED_PLAYBOOK rooted at $BOOST_SRC_DIR"
    perl -pe "s{^(\s*)- url: \.\.\s*\$}{\$1- url: $BOOST_SRC_DIR\n};
              s{^(\s*)start_path: doc\s*\$}{\$1start_path: $REL_START_PATH\n}" \
      "$PLAYBOOK" > "$DERIVED_PLAYBOOK"
    PLAYBOOK="$DERIVED_PLAYBOOK"
  else
    echo "WARNING: libs/test/.git is a file (submodule) and no usable BOOST_SRC_DIR" >&2
    echo "         was found; Antora will reject the content source." >&2
  fi
fi

echo "Building documentation with Antora..."
echo "Installing npm dependencies..."
npm ci

echo "Building docs in custom dir..."
PATH="$(pwd)/node_modules/.bin:${PATH}"
export PATH

# The reference pages link each header to its source with `link:{base-url}/...`.
# Point that at the exact commit when we know it; otherwise antora.yml's
# fallback applies. A command-line attribute outranks the one in antora.yml.
ANTORA_ARGS=()
if [ -n "${BASE_URL:-}" ]; then
  ANTORA_ARGS+=(--attribute "base-url=$BASE_URL")
fi

# Antora exits 0 even when xrefs and includes fail to resolve. Make that fatal
# in CI, but keep local previews usable while pages are being worked on.
if [ -n "${CI:-}" ]; then
  ANTORA_ARGS+=(--log-failure-level=warn)
fi

npx antora --clean --fetch "$PLAYBOOK" "${ANTORA_ARGS[@]}" --stacktrace # --log-level all

echo "Fixing links to non-mrdocs URIs..."
echo "BRANCH='${BRANCH:-}'"
echo "BASE_URL='${BASE_URL:-}'"

for f in $(find html -name '*.html'); do
  perl -i -pe "s{<a href=\"index.html\">Boost.Test</a>}{<a href=\"https://www.boost.org/library/${BRANCH}/test/\">Boost.Test</a>}g" "$f"
done

if [ -n "${BASE_URL:-}" ]; then
  if [ -f mrdocs.yml.bak ]; then
    mv -f mrdocs.yml.bak mrdocs.yml
    echo "Restored original mrdocs.yml"
  else
    echo "mrdocs.yml.bak not found; skipping restore"
  fi
fi

echo "Done"
