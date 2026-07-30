#!/bin/bash
#
# Copyright (c) 2026 Jean-Louis Leroy
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Decide whether the cached MrDocs binary is stale and, if so, clear it so the
# next Antora build downloads the current nightly.
#
# Why this is needed: build_antora.sh runs mrdocs through the
# @cppalliance/antora-cpp-reference-extension. With no `version` pinned in
# local-playbook.yml the extension resolves "*", which is the rolling
# `develop-release` nightly. It caches the download under a directory keyed by
# the *tag name* (.../mrdocs/<os>/develop/), so once that directory exists it is
# reused indefinitely and never tracks newer nightlies. CI has no such problem
# because every runner starts with an empty cache.
#
# This script compares the upstream `develop-release` asset's publish time to
# the value we last synced to (recorded in a marker file). When upstream is
# newer -- or nothing is cached yet -- it removes the cache directory. It is a
# safe no-op when offline, rate-limited, or when a concrete version is pinned.

set -u

# Only the floating `develop`/`master` caches go stale; a pinned semver build is
# reproducible and must never be cleared. Skip unless we are tracking a nightly.
TAG="${MRDOCS_CACHE_TAG:-develop}"
case "$TAG" in
  develop | master) ;;
  *) echo "MrDocs cache tag '$TAG' is pinned; nothing to refresh."; exit 0 ;;
esac

# Cache layout used by the reference extension: <cache>/<os>/<tag>/bin/mrdocs
case "$(uname -s)" in
  Linux)  OS=linux  ;;
  Darwin) OS=darwin ;;
  *)      echo "Unsupported OS for cache refresh; leaving cache untouched."; exit 0 ;;
esac

CACHE_ROOT="${HOME}/.cache/antora/reference-collector/mrdocs/${OS}"
CACHE_DIR="${CACHE_ROOT}/${TAG}"
BIN="${CACHE_DIR}/bin/mrdocs"
MARKER="${CACHE_ROOT}/${TAG}.upstream" # records the upstream timestamp we synced

clear_cache() { # $1 = reason
  echo "Clearing MrDocs cache (${1}): ${CACHE_DIR}"
  rm -rf "$CACHE_DIR"
}

# Nothing cached yet -> let the extension download; record nothing.
if [ ! -x "$BIN" ]; then
  echo "No cached MrDocs '${TAG}' build; the Antora build will download it."
  exit 0
fi

# Ask GitHub when the develop-release Linux/macOS asset was last published.
AUTH=()
[ -n "${GITHUB_TOKEN:-}" ] && AUTH=(-H "Authorization: Bearer ${GITHUB_TOKEN}")
ASSET_SUFFIX="$([ "$OS" = darwin ] && echo Darwin || echo Linux).tar.gz"

UPSTREAM_TS=$(curl -fsSL --max-time 15 "${AUTH[@]}" \
  "https://api.github.com/repos/cppalliance/mrdocs/releases/tags/${TAG}-release" 2>/dev/null \
  | ASSET_SUFFIX="$ASSET_SUFFIX" python3 -c '
import json, os, sys
try:
    data = json.load(sys.stdin)
except Exception:
    sys.exit(0)
suffix = os.environ["ASSET_SUFFIX"]
for a in data.get("assets", []):
    if a.get("name", "").endswith(suffix):
        print(a.get("updated_at", ""))
        break
' 2>/dev/null)

if [ -z "$UPSTREAM_TS" ]; then
  echo "Could not query upstream MrDocs (offline / rate-limited); keeping cached build."
  exit 0
fi

LAST_TS="$(cat "$MARKER" 2>/dev/null || true)"

if [ -z "$LAST_TS" ]; then
  # First run against an existing cache: we cannot know its true upstream age,
  # so adopt it as the baseline rather than forcing a surprise re-download.
  # (Run `rm -rf "$CACHE_DIR"` once by hand if you suspect it is already stale.)
  printf '%s\n' "$UPSTREAM_TS" > "$MARKER"
  echo "Recording MrDocs '${TAG}' baseline (${UPSTREAM_TS}); keeping current cache."
elif [ "$UPSTREAM_TS" != "$LAST_TS" ]; then
  clear_cache "upstream ${TAG}-release published ${UPSTREAM_TS}, cached ${LAST_TS}"
  # Record what the next build will fetch so we do not clear again until it moves.
  printf '%s\n' "$UPSTREAM_TS" > "$MARKER"
else
  echo "Cached MrDocs '${TAG}' nightly is current (${UPSTREAM_TS})."
fi
