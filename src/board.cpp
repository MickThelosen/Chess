#include "include\board.h"

int Board::setupBoard()
{
    int empties = 16;

    // Pawns
    for (int i = 0; i < 8; i++)
    {
        board[1][i] = {'P', 8 + i, true};
        board[6][i] = {'P', 48 + i, false};
    }

    // Empty squares
    for (int i = 2; i < 6; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[i][j] = {'E', empties++, true};
        }
    }

    // Rooks
    board[0][0] = {'R', 0, true};
    board[0][7] = {'R', 7, true};
    board[7][0] = {'R', 56, false};
    board[7][7] = {'R', 63, false};

    // Knights 'N'
    board[0][1] = {'N', 1, true};
    board[0][6] = {'N', 6, true};
    board[7][1] = {'N', 57, false};
    board[7][6] = {'N', 62, false};

    // Bishops
    board[0][2] = {'B', 2, true};
    board[0][5] = {'B', 5, true};
    board[7][2] = {'B', 58, false};
    board[7][5] = {'B', 61, false};

    // Queens
    board[0][3] = {'Q', 3, true};
    board[7][3] = {'Q', 59, false};

    // Kings
    board[0][4] = {'K', 4, true};
    board[7][4] = {'K', 60, false};
}

using namespace sf;
using namespace std;

void Board::displayBoard(RenderWindow &window, Texture &chessPiecesTexture) const
{
    Sprite chessboardSprite(chessPiecesTexture);
    Sprite pieceSprite;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            int squareSize = 100; // Assuming each square is 100x100 pixels

            // Set the chessboard square position
            chessboardSprite.setPosition(j * squareSize, i * squareSize);

            // Draw the chessboard square
            window.draw(chessboardSprite);

            // Draw the piece if it is not an empty square
            if (board[i][j].type != 'E')
            {
                int pieceSize = 100; // Assuming each piece is 100x100 pixels

                // Set the piece position
                pieceSprite.setPosition(j * squareSize, i * squareSize);

                // Set the texture rectangle based on the piece type and color
                int textureX = (board[i][j].type - 'A') * pieceSize;
                int textureY = board[i][j].isWhite ? 0 : pieceSize;

                pieceSprite.setTextureRect(IntRect(textureX, textureY, pieceSize, pieceSize));

                // Draw the piece
                window.draw(pieceSprite);
            }
        }
    }
}

void Board::drawPNGPiece(RenderWindow &window, Texture &texture, char pieceType, bool isWhite, Vector2f position, float size) const
{
    string filename = (isWhite ? "W" : "B") + string(1, pieceType) + ".png";

    if (texture.loadFromFile(filename))
    {
        Sprite sprite(texture);
        sprite.setPosition(position);
        sprite.setScale(size / texture.getSize().x, size / texture.getSize().y);
        window.draw(sprite);
    }
    else
    {
    }
}