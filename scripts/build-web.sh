#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$ROOT/build-web}"
DIST_DIR="${DIST_DIR:-$ROOT/dist-web}"
CROSS_FILE="$ROOT/.github/emscripten-ghactions.ini"

for tool in emcc em++ emar emstrip python3; do
  if ! command -v "$tool" >/dev/null 2>&1; then
    echo "Missing '$tool'. Activate the Emscripten SDK environment first (source <emsdk>/emsdk_env.sh)." >&2
    exit 2
  fi
done
if ! command -v meson >/dev/null 2>&1; then
  echo "Meson is missing. Install it with: python3 -m pip install --user 'meson>=0.64.0'" >&2
  exit 2
fi

if [[ -f "$BUILD_DIR/build.ninja" ]]; then
  meson setup "$BUILD_DIR" --reconfigure \
    --cross-file "$CROSS_FILE" \
    -Dstatic=prebuilt -Doptimization=2 -Dresolve_vcs_tag=no
else
  meson setup "$BUILD_DIR" \
    --cross-file "$CROSS_FILE" \
    -Dstatic=prebuilt -Doptimization=2 -Dresolve_vcs_tag=no
fi

meson compile -C "$BUILD_DIR" --jobs "${JOBS:-2}"

mkdir -p "$DIST_DIR"
cp "$ROOT/web/index.html" "$DIST_DIR/index.html"
for file in powder.js powder.wasm powder.wasm.map; do
  if [[ -f "$BUILD_DIR/$file" ]]; then
    cp "$BUILD_DIR/$file" "$DIST_DIR/$file"
  fi
done
if [[ ! -s "$DIST_DIR/powder.js" || ! -s "$DIST_DIR/powder.wasm" ]]; then
  echo "Build completed without the expected powder.js/powder.wasm outputs." >&2
  exit 3
fi

echo
echo "Web build ready: $DIST_DIR"
echo "Serve it over HTTP(S), for example: python3 -m http.server 8000 --directory \"$DIST_DIR\""
