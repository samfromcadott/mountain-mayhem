emcc -o src/index.html src/*.cc -std=c++17 -Os -Wall -lraylib -Iinclude -L. -Llib/web -s USE_GLFW=3 -s WASM=0 --shell-file src/minshell.html -DPLATFORM_WEB
