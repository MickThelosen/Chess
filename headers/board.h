#ifndef Board_H
#define Board_H

#include <iostream>
#include <map>
#include <cstring>
#include <../SDL2/SDL.h>
#include <../SDL2/SDL_image.h>
#include <../SDL2/SDL_ttf.h>

using namespace std;

struct Piece
{
    char type;
    bool isWhite;
    bool hasMoved = false;
    bool isSelected = false;
    bool possible = false;
    bool castlePath = false;
    bool castleSquare = false;
    bool checksKing = false;
};

class Board
{
private:
    Piece board[8][8];
    const int SCREEN_WIDTH = 920;
    const int SCREEN_HEIGHT = 736;
    const int SQUARE_SIZE_WIDTH = SCREEN_WIDTH / 10;
    const int SQUARE_SIZE_HEIGHT = SCREEN_HEIGHT / 8;
    SDL_Window *gWindow = NULL;
    SDL_Renderer *gRenderer = NULL;
    SDL_Window *debugWindow = NULL;
    SDL_Renderer *debugRenderer = NULL;
    std::map<std::string, SDL_Texture*> gPieceTextures;
    int selectedX, selectedY;
    int wScore = 0;
    int bScore = 0;
    bool whiteTurn = true;
    bool pieceSelected = false;
    bool castleSelected = false;
    int gameStatus = 0; // 0 = stopped 1 = playing 2 = white check 3 = black check 4 = white win 5 = black win
    char score[100];
    char gameInfo[100];
    char gameCheck[100];
    int selectedPiece[2] = {0 , 0};
public:
    Board(){};
    void setupBoard();
    bool initSDL();
    void closeSDL();
    void drawChessboard();
    bool loadMedia();
    void calcMoves(int i, int j);
    void setSelected(int x, int y);
    bool onBoard(int x, int y);
    int gameState();
    void printHasMoved();
    void printType();
    void printSelected();
    void printCastlePath();
    SDL_Texture* loadTexture(const std::string& path);
};

#endif