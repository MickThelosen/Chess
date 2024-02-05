#ifndef Board_H
#define Board_H

#include <iostream>
#include <map>
#include <cstring>
#include <Windows.h>
#include <../SDL2/SDL.h>
#include <../SDL2/SDL_image.h>

using namespace std;

struct Piece
{
    char type = 'O';
    int pos;
    bool isWhite;
};

class Board
{
private:
    Piece board[8][8];
    const int SCREEN_WIDTH = 736;
    const int SCREEN_HEIGHT = 736;
    const int SQUARE_SIZE = SCREEN_WIDTH / 8;
    SDL_Window *gWindow = NULL;
    SDL_Renderer *gRenderer = NULL;
    std::map<std::string, SDL_Texture*> gPieceTextures;

public:
    Board(){};
    void setupBoard();
    bool initSDL();
    void closeSDL();
    void drawChessboard();
    bool loadMedia();
    void createDebugConsole();
    SDL_Texture* loadTexture(const std::string& path);
};

#endif