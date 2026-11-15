# The Powder Toy — browser build

This folder adds a browser target to the supplied upstream source tree. The game's simulation, rendering, controls and interface remain in the original C++/SDL code. Emscripten compiles that code to JavaScript + WebAssembly; `web/index.html` is only a minimal page that loads the original game canvas and scales it to the available screen without redesigning the in-game UI.

## What is included

- `web/index.html` — browser launcher for the generated `powder.js` and `powder.wasm`.
- `scripts/build-web.sh` — Linux/macOS build script.
- `scripts/build-web.bat` — Windows build script.
- `.github/emscripten-ghactions.ini` — cross-compilation file already present in the upstream project.
- `src/` and the rest of the source tree — unchanged original game source.

## Requirements

1. Install and activate the Emscripten SDK so `emcc`, `em++`, `emar` and `emstrip` are on `PATH`.
2. Install Meson 0.64 or newer and Ninja.
3. Have an internet connection on the build computer. Meson downloads the project's prebuilt static dependency bundle on the first build.

## Build on Linux / macOS

From the project root, with the Emscripten SDK activated:

```bash
python3 -m pip install --user 'meson>=0.64.0' ninja
./scripts/build-web.sh
```

## Build on Windows

Open the Emscripten Command Prompt (or run `emsdk_env.bat`), then:

```bat
py -m pip install meson ninja
scripts\build-web.bat
```

The resulting browser files are placed in `dist-web/`.

## Run locally

Serve `dist-web/` over HTTP rather than opening `index.html` as a `file://` URL:

```bash
python3 -m http.server 8000 --directory dist-web
```

Then open `http://localhost:8000/`. On a phone, the computer and phone must be on the same local network and you must use the computer's LAN address instead of `localhost`.

## Build status in this workspace

The source archive contains the Emscripten cross file and Emscripten-specific SDL main-loop implementation. However, this workspace does not have `emcc`/`em++` or Meson installed and cannot reach external package hosts. Therefore no compiled `.wasm` binary is included here. The scripts are prepared for a machine with the requirements above; this package is not being represented as an already compiled playable build.

## License

The upstream project is distributed under GNU GPL-3.0. See `LICENSE` and retain it with any modified or redistributed build. This browser wrapper is an addition to that source tree, not a replacement for the upstream license.
