@echo off
setlocal
set "ROOT=%~dp0.."
set "BUILD_DIR=%ROOT%\build-web"
set "DIST_DIR=%ROOT%\dist-web"
where emcc >nul 2>nul || (echo Emscripten is not active. Run emsdk_env.bat first. & exit /b 2)
where em++ >nul 2>nul || (echo em++ was not found. & exit /b 2)
where meson >nul 2>nul || (echo Install Meson: py -m pip install meson & exit /b 2)
if exist "%BUILD_DIR%\build.ninja" (
  meson setup "%BUILD_DIR%" --reconfigure --cross-file "%ROOT%\.github\emscripten-ghactions.ini" -Dstatic=prebuilt -Doptimization=2 -Dresolve_vcs_tag=no
) else (
  meson setup "%BUILD_DIR%" --cross-file "%ROOT%\.github\emscripten-ghactions.ini" -Dstatic=prebuilt -Doptimization=2 -Dresolve_vcs_tag=no
)
if errorlevel 1 exit /b %errorlevel%
meson compile -C "%BUILD_DIR%" --jobs 2
if errorlevel 1 exit /b %errorlevel%
if not exist "%DIST_DIR%" mkdir "%DIST_DIR%"
copy /Y "%ROOT%\web\index.html" "%DIST_DIR%\index.html" >nul
for %%F in (powder.js powder.wasm powder.wasm.map) do if exist "%BUILD_DIR%\%%F" copy /Y "%BUILD_DIR%\%%F" "%DIST_DIR%\%%F" >nul
if not exist "%DIST_DIR%\powder.js" (echo Build did not create powder.js.& exit /b 3)
if not exist "%DIST_DIR%\powder.wasm" (echo Build did not create powder.wasm.& exit /b 3)
echo Web build ready in %DIST_DIR%
echo Serve with: py -m http.server 8000 --directory "%DIST_DIR%"
