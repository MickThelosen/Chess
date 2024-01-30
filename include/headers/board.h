#ifndef Board_H
#define Board_H
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;
struct Piece {
    char type;
    int pos;
    bool isWhite;
};

class Board {
    private:
    Piece board[8][8];
    map<pair<char, bool>, Texture> pieceTextures;
    public:
    Board() {};
    void setupBoard();
    void displayBoard(RenderWindow &window) const;
};

#endif