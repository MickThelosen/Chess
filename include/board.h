#ifndef Board_H
#define Board_H
#include <iostream>
#include <SMFL\include\SFML\Graphics.hpp>
using namespace sf;

struct Piece {
    char type;
    int pos;
    bool isWhite;
};

class Board {
    private:
    Piece board[8][8];
    void drawPNGPiece(sf::RenderWindow& window, sf::Texture& texture, char pieceType, bool isWhite, sf::Vector2f position, float size) const;
    public:
    Board() {setupBoard();};
    int setupBoard();
    void displayBoard(RenderWindow& window, Texture& chessPiecesTexture) const;
};

#endif