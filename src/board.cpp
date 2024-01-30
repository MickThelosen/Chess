#include "../include/headers/board.h"
using namespace sf;
using namespace std;

void Board::setupBoard()
{
    int empties = 16;

    for (int i = 0; i < 8; i++)
    {
        board[1][i] = {'P', 8 + i, true};
        board[6][i] = {'P', 48 + i, false};
    }

    for (int i = 2; i < 6; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[i][j] = {'E', empties++, true};
        }
    }

    board[0][0] = {'R', 0, true};
    board[0][7] = {'R', 7, true};
    board[7][0] = {'R', 56, false};
    board[7][7] = {'R', 63, false};

    board[0][1] = {'N', 1, true};
    board[0][6] = {'N', 6, true};
    board[7][1] = {'N', 57, false};
    board[7][6] = {'N', 62, false};

    board[0][2] = {'B', 2, true};
    board[0][5] = {'B', 5, true};
    board[7][2] = {'B', 58, false};
    board[7][5] = {'B', 61, false};

    board[0][3] = {'Q', 3, true};
    board[7][3] = {'Q', 59, false};

    board[0][4] = {'K', 4, true};
    board[7][4] = {'K', 60, false};

    for (char pieceType : {'P', 'R', 'N', 'B', 'Q', 'K'})
    {
        for (bool isWhite : {true, false})
        {
            string filename = string("../assets/chess/") + (isWhite ? "W" : "B") + string(1, pieceType) + ".png";
            Texture pieceTexture;

            if (pieceTexture.loadFromFile(filename))
            {
                pieceTextures[make_pair(pieceType, isWhite)] = pieceTexture;
                cout << "Loaded: " << filename << endl;
            }
            else
            {
                cout << "Error loading texture: " << filename << endl;
            }
        }
    }
}

void Board::displayBoard(RenderWindow &window) const
{
    Sprite chessboardSprite(pieceTextures.at({'E', true}));
    Sprite pieceSprite;

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            int squareSize = 60;

            chessboardSprite.setPosition(j * squareSize, i * squareSize);

            window.draw(chessboardSprite);

            if (board[i][j].type != 'E')
            {
                int pieceSize = 60;

                pieceSprite.setPosition(j * squareSize, i * squareSize);

                auto textureIt = pieceTextures.find({board[i][j].type, board[i][j].isWhite});
                if (textureIt != pieceTextures.end())
                {
                    const Texture &pieceTexture = textureIt->second;

                    pieceSprite.setTexture(pieceTexture);
                    pieceSprite.setTextureRect(IntRect(0, 0, pieceSize, pieceSize));

                    window.draw(pieceSprite);
                }
                else
                {
                    cout << "Error: Texture not found for piece " << board[i][j].type << endl;
                }
            }
        }
    }
}