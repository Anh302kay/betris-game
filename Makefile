release:
	g++ -O3 -g -std=c++20 -Wall source/*.cpp -I include -I res -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -mwindows -o bin/release/game.exe
debug:
	g++ -g -std=c++20 -Wall source/*.cpp -I include -I res -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -mwindows -static-libstdc++ -o bin/debug/game.exe

web:
	em++ -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_MIXER=2 -sMAX_WEBGL_VERSION=2 -sMIN_WEBGL_VERSION=2 -std=c++20 -Iinclude -Ires source/main.cpp source/tetromino.cpp source/game.cpp source/renderwindow.cpp source/entity.cpp --preload-file .\res -o b.html --use-preload-plugins -mnontrapping-fptoint -s ALLOW_MEMORY_GROWTH=1