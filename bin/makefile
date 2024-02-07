all: main

main: 
	g++ -g --std=c++17 -I../include/SDL2 -I../headers -L../lib ../src/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o Chess

del:
	del Chess.exe