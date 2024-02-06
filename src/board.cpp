#include "../headers/board.h"

using namespace std;

void Board::setupBoard()
{
    gameStatus = 1;
    for (int i = 0; i < 8;)
    {
        board[1][i] = {'P', false};
        board[6][i] = {'P', true};
        i++;
    }

    for (int i = 2; i < 6; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[i][j] = {'E'};
        }
    }

    board[0][0] = {'R', false};
    board[0][7] = {'R', false};
    board[7][0] = {'R', true};
    board[7][7] = {'R', true};

    board[0][1] = {'N', false};
    board[0][6] = {'N', false};
    board[7][1] = {'N', true};
    board[7][6] = {'N', true};

    board[0][2] = {'B', false};
    board[0][5] = {'B', false};
    board[7][2] = {'B', true};
    board[7][5] = {'B', true};

    board[0][3] = {'Q', false};
    board[7][3] = {'Q', true};

    board[0][4] = {'K', false};
    board[7][4] = {'K', true};
}

bool Board::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return false;
    }

    gWindow = SDL_CreateWindow("Let's go golfing!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL)
    {
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        return false;
    }

    return true;
}

bool Board::loadMedia()
{

    gPieceTextures["BB"] = IMG_LoadTexture(gRenderer, "../assets/BB.png");
    gPieceTextures["BK"] = IMG_LoadTexture(gRenderer, "../assets/BK.png");
    gPieceTextures["BN"] = IMG_LoadTexture(gRenderer, "../assets/BN.png");
    gPieceTextures["BP"] = IMG_LoadTexture(gRenderer, "../assets/BP.png");
    gPieceTextures["BQ"] = IMG_LoadTexture(gRenderer, "../assets/BQ.png");
    gPieceTextures["BR"] = IMG_LoadTexture(gRenderer, "../assets/BR.png");
    gPieceTextures["WB"] = IMG_LoadTexture(gRenderer, "../assets/WB.png");
    gPieceTextures["WK"] = IMG_LoadTexture(gRenderer, "../assets/WK.png");
    gPieceTextures["WN"] = IMG_LoadTexture(gRenderer, "../assets/WN.png");
    gPieceTextures["WP"] = IMG_LoadTexture(gRenderer, "../assets/WP.png");
    gPieceTextures["WQ"] = IMG_LoadTexture(gRenderer, "../assets/WQ.png");
    gPieceTextures["WR"] = IMG_LoadTexture(gRenderer, "../assets/WR.png");

    for (const auto &pair : gPieceTextures)
    {
        if (pair.second == NULL)
        {
            cout << "Error loading texture: " << pair.first << endl;
            return false;
        }
    }

    return true;
}

SDL_Texture *Board::loadTexture(const std::string &path)
{
    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        cout << "Unable to load image " << path.c_str() << "! SDL_image Error: " << IMG_GetError() << endl;
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            cout << "Unable to create texture from " << path.c_str() << "! SDL Error: " << SDL_GetError() << endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void Board::closeSDL()
{
    for (auto &pair : gPieceTextures)
    {
        SDL_DestroyTexture(pair.second);
    }

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    IMG_Quit();
    SDL_Quit();
    gameStatus = 0;
}

void Board::drawChessboard()
{
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            SDL_Rect squareRect = {j * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
            if ((i + j) % 2 == 0)
            {
                SDL_SetRenderDrawColor(gRenderer, 139, 69, 19, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            }
            if (board[i][j].possible && (i + j) % 2 == 0)
            {
                SDL_SetRenderDrawColor(gRenderer, 100, 100, 100, 255);
            }
            else if (board[i][j].possible && (i + j) % 2 != 0)
            {
                SDL_SetRenderDrawColor(gRenderer, 150, 150, 150, 255);
            }
            if (board[i][j].isSelected && (i + j) % 2 == 0)
            {
                SDL_SetRenderDrawColor(gRenderer, 75, 75, 75, 255);
            }
            else if (board[i][j].isSelected && (i + j) % 2 != 0)
            {
                SDL_SetRenderDrawColor(gRenderer, 115, 115, 115, 255);
            }
            SDL_RenderFillRect(gRenderer, &squareRect);
        }
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j].type != 'E')
            {
                string texture = "";
                board[i][j].isWhite ? texture += "W" : texture += "B";
                texture += board[i][j].type;
                SDL_Rect pieceRect = {j * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
                SDL_RenderCopy(gRenderer, gPieceTextures[texture], NULL, &pieceRect);
            }
        }
    }
    SDL_RenderPresent(gRenderer);
}

void Board::calcMoves()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j].isSelected == true)
            {
                switch (board[i][j].type)
                {
                case 'P':
                    if (board[i][j].isWhite)
                    {
                        if (board[i - 1][j].type == 'E')
                        {
                            board[i - 1][j].possible = true;
                        }
                        if (board[i - 2][j].type == 'E' && !board[i][j].hasMoved)
                        {
                            board[i - 2][j].possible = true;
                        }
                        if (board[i - 1][j - 1].type != 'E' && !board[i - 1][j - 1].isWhite && onBoard(i - 1, j - 1))
                        {
                            board[i - 1][j - 1].possible = true;
                        }
                        if (board[i - 1][j + 1].type != 'E' && !board[i - 1][j + 1].isWhite && onBoard(i - 1, j + 1))
                        {
                            board[i - 1][j + 1].possible = true;
                        }
                    }
                    else if (!board[i][j].isWhite)
                    {
                        if (board[i + 1][j].type == 'E')
                        {
                            board[i + 1][j].possible = true;
                        }
                        if (board[i + 2][j].type == 'E' && !board[i][j].hasMoved)
                        {
                            board[i + 2][j].possible = true;
                        }
                        if (board[i + 1][j - 1].type != 'E' && board[i + 1][j - 1].isWhite && onBoard(i + 1, j - 1))
                        {
                            board[i + 1][j - 1].possible = true;
                        }
                        if (board[i + 1][j + 1].type != 'E' && board[i + 1][j + 1].isWhite && onBoard(i + 1, j + 1))
                        {
                            board[i + 1][j + 1].possible = true;
                        }
                    }
                    break;
                case 'R':
                    for (int index = i - 1; index >= 0; index--)
                    {
                        if (board[index][j].type != 'E')
                        {
                            if (board[index][j].isWhite != board[i][j].isWhite)
                            {
                                board[index][j].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[index][j].possible = true;
                        }
                    }
                    for (int index = i + 1; index < 8; index++)
                    {
                        if (board[index][j].type != 'E')
                        {
                            if (board[index][j].isWhite != board[i][j].isWhite)
                            {
                                board[index][j].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[index][j].possible = true;
                        }
                    }
                    for (int index = j - 1; index >= 0; index--)
                    {
                        if (board[i][index].type != 'E')
                        {
                            if (board[i][index].isWhite != board[i][j].isWhite)
                            {
                                board[i][index].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[i][index].possible = true;
                        }
                    }
                    for (int index = j + 1; index < 8; index++)
                    {
                        if (board[i][index].type != 'E')
                        {
                            if (board[i][index].isWhite != board[i][j].isWhite)
                            {
                                board[i][index].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[i][index].possible = true;
                        }
                    }
                    break;
                case 'B':
                    for (int indexI = i - 1, indexJ = j - 1; indexI >= 0 && indexI >= 0; indexI--, indexJ--)
                    {
                        if (!onBoard(indexI, indexJ))
                        {
                            break;
                        }
                        if (board[indexI][indexJ].type != 'E')
                        {
                            if (board[indexI][indexJ].isWhite != board[i][j].isWhite)
                            {
                                board[indexI][indexJ].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[indexI][indexJ].possible = true;
                        }
                    }
                    for (int indexI = i - 1, indexJ = j + 1; indexI >= 0 && indexI < 8; indexI--, indexJ++)
                    {
                        if (!onBoard(indexI, indexJ))
                        {
                            break;
                        }
                        if (board[indexI][indexJ].type != 'E')
                        {
                            if (board[indexI][indexJ].isWhite != board[i][j].isWhite)
                            {
                                board[indexI][indexJ].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[indexI][indexJ].possible = true;
                        }
                    }
                    for (int indexI = i + 1, indexJ = j + 1; indexI < 8 && indexI < 8; indexI++, indexJ++)
                    {
                        if (!onBoard(indexI, indexJ))
                        {
                            break;
                        }
                        if (board[indexI][indexJ].type != 'E')
                        {
                            if (board[indexI][indexJ].isWhite != board[i][j].isWhite)
                            {
                                board[indexI][indexJ].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[indexI][indexJ].possible = true;
                        }
                    }
                    for (int indexI = i + 1, indexJ = j - 1; indexI < 8 && indexI >= 0; indexI++, indexJ--)
                    {
                        if (!onBoard(indexI, indexJ))
                        {
                            break;
                        }
                        if (board[indexI][indexJ].type != 'E')
                        {
                            if (board[indexI][indexJ].isWhite != board[i][j].isWhite)
                            {
                                board[indexI][indexJ].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[indexI][indexJ].possible = true;
                        }
                    }
                    break;
                case 'N':
                    if (board[i - 2][j - 1].isWhite != board[i][j].isWhite && onBoard(i - 2, j - 1) || board[i - 2][j - 1].type == 'E' && onBoard(i - 2, j - 1))
                    {
                        board[i - 2][j - 1].possible = true;
                    }
                    if (board[i - 2][j + 1].isWhite != board[i][j].isWhite && onBoard(i - 2, j + 1) || board[i - 2][j + 1].type == 'E' && onBoard(i - 2, j + 1))
                    {
                        board[i - 2][j + 1].possible = true;
                    }
                    if (board[i + 2][j - 1].isWhite != board[i][j].isWhite && onBoard(i + 2, j - 1) || board[i + 2][j - 1].type == 'E' && onBoard(i + 2, j - 1))
                    {
                        board[i + 2][j - 1].possible = true;
                    }
                    if (board[i + 2][j + 1].isWhite != board[i][j].isWhite && onBoard(i + 2, j + 1) || board[i + 2][j + 1].type == 'E' && onBoard(i + 2, j + 1))
                    {
                        board[i + 2][j + 1].possible = true;
                    }
                    if (board[i - 1][j - 2].isWhite != board[i][j].isWhite && onBoard(i - 1, j - 2) || board[i - 1][j - 2].type == 'E' && onBoard(i - 1, j - 2))
                    {
                        board[i - 1][j - 2].possible = true;
                    }
                    if (board[i - 1][j + 2].isWhite != board[i][j].isWhite && onBoard(i - 1, j + 2) || board[i - 1][j + 2].type == 'E' && onBoard(i - 1, j + 2))
                    {
                        board[i - 1][j + 2].possible = true;
                    }
                    if (board[i + 1][j - 2].isWhite != board[i][j].isWhite && onBoard(i + 1, j - 2) || board[i + 1][j - 2].type == 'E' && onBoard(i + 1, j - 2))
                    {
                        board[i + 1][j - 2].possible = true;
                    }
                    if (board[i + 1][j + 2].isWhite != board[i][j].isWhite && onBoard(i + 1, j + 2) || board[i + 1][j + 2].type == 'E' && onBoard(i + 1, j + 2))
                    {
                        board[i + 1][j + 2].possible = true;
                    }
                    break;
                case 'Q':
                    for (int index = i - 1; index >= 0; index--)
                    {
                        if (board[index][j].type != 'E')
                        {
                            if (board[index][j].isWhite != board[i][j].isWhite)
                            {
                                board[index][j].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[index][j].possible = true;
                        }
                    }
                    for (int index = i + 1; index < 8; index++)
                    {
                        if (board[index][j].type != 'E')
                        {
                            if (board[index][j].isWhite != board[i][j].isWhite)
                            {
                                board[index][j].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[index][j].possible = true;
                        }
                    }
                    for (int index = j - 1; index >= 0; index--)
                    {
                        if (board[i][index].type != 'E')
                        {
                            if (board[i][index].isWhite != board[i][j].isWhite)
                            {
                                board[i][index].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[i][index].possible = true;
                        }
                    }
                    for (int index = j + 1; index < 8; index++)
                    {
                        if (board[i][index].type != 'E')
                        {
                            if (board[i][index].isWhite != board[i][j].isWhite)
                            {
                                board[i][index].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[i][index].possible = true;
                        }
                    }
                    for (int indexI = i - 1, indexJ = j - 1; indexI >= 0 && indexI >= 0; indexI--, indexJ--)
                    {
                        if (!onBoard(indexI, indexJ))
                        {
                            break;
                        }
                        if (board[indexI][indexJ].type != 'E')
                        {
                            if (board[indexI][indexJ].isWhite != board[i][j].isWhite)
                            {
                                board[indexI][indexJ].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[indexI][indexJ].possible = true;
                        }
                    }
                    for (int indexI = i - 1, indexJ = j + 1; indexI >= 0 && indexI < 8; indexI--, indexJ++)
                    {
                        if (!onBoard(indexI, indexJ))
                        {
                            break;
                        }
                        if (board[indexI][indexJ].type != 'E')
                        {
                            if (board[indexI][indexJ].isWhite != board[i][j].isWhite)
                            {
                                board[indexI][indexJ].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[indexI][indexJ].possible = true;
                        }
                    }
                    for (int indexI = i + 1, indexJ = j + 1; indexI < 8 && indexI < 8; indexI++, indexJ++)
                    {
                        if (!onBoard(indexI, indexJ))
                        {
                            break;
                        }
                        if (board[indexI][indexJ].type != 'E')
                        {
                            if (board[indexI][indexJ].isWhite != board[i][j].isWhite)
                            {
                                board[indexI][indexJ].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[indexI][indexJ].possible = true;
                        }
                    }
                    for (int indexI = i + 1, indexJ = j - 1; indexI < 8 && indexI >= 0; indexI++, indexJ--)
                    {
                        if (!onBoard(indexI, indexJ))
                        {
                            break;
                        }
                        if (board[indexI][indexJ].type != 'E')
                        {
                            if (board[indexI][indexJ].isWhite != board[i][j].isWhite)
                            {
                                board[indexI][indexJ].possible = true;
                            }
                            break;
                        }
                        else
                        {
                            board[indexI][indexJ].possible = true;
                        }
                    }
                    break;
                case 'K':
                    for (int _i = i - 1; _i <= i + 1; _i++)
                    {
                        for (int _j = j - 1; _j <= j + 1; _j++)
                        {
                            if (board[_i][_j].isWhite != board[i][j].isWhite && onBoard(_i, _j) || board[_i][_j].type == 'E' && onBoard(_i, _j))
                            {
                                board[_i][_j].possible = true;
                            }
                        }
                    }
                    break;
                case 'E':
                    break;
                default:
                    cout << "Error" << endl;
                    break;
                }
            }
        }
    }
}

void Board::setSelected(int X, int Y)
{
    int squareSize = SCREEN_WIDTH / 8;
    selectedX = X / squareSize;
    selectedY = Y / squareSize;

    if (board[selectedY][selectedX].type != 'E' && whiteTurn && board[selectedY][selectedX].isWhite || board[selectedY][selectedX].type != 'E' && !whiteTurn && !board[selectedY][selectedX].isWhite)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                board[i][j].isSelected = false;
                board[i][j].possible = false;
            }
        }
        board[selectedY][selectedX].isSelected = true;
        pieceSelected = true;
    }
    if (board[selectedY][selectedX].type == 'E' && pieceSelected && board[selectedY][selectedX].possible || !board[selectedY][selectedX].isWhite && whiteTurn && pieceSelected && board[selectedY][selectedX].possible || board[selectedY][selectedX].isWhite && !whiteTurn && pieceSelected && board[selectedY][selectedX].possible)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (board[i][j].isSelected)
                {
                    board[selectedY][selectedX].type = board[i][j].type;
                    board[selectedY][selectedX].isWhite = board[i][j].isWhite;
                    board[selectedY][selectedX].hasMoved = true;
                    board[i][j].type = 'E';
                    board[i][j].isWhite = true;
                    pieceSelected = false;
                }
            }
        }
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                board[i][j].isSelected = false;
                board[i][j].possible = false;
            }
        }
        whiteTurn = !whiteTurn;
    }
    cout << "Selected type: " << board[selectedY][selectedX].type << " at y: " << selectedY << " x: " << selectedX << endl;
}

bool Board::onBoard(int x, int y)
{
    if (x >= 0 && x <= 7 && y >= 0 && y <= 7)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Board::gameState()
{
    int status = 1;
    bool bKingAlive, wKingAlive = false;
    if (!pieceSelected) {
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j].isWhite)
            {
                board[i][j].isSelected = true;
                calcMoves();
            }
            for (int a = 0; a < 8; a++)
            {
                for (int b = 0; b < 8; b++)
                {
                    if (board[a][b].type == 'K' && board[a][b].possible && !board[a][b].isWhite)
                    {
                        cout << "White checked black" << endl;
                        status = 2;
                    }
                    board[a][b].isSelected = false;
                    board[a][b].possible = false;
                }
            }
                }
    }
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (!board[i][j].isWhite)
            {
                board[i][j].isSelected = true;
                calcMoves();
            }
            for (int a = 0; a < 8; a++)
            {
                for (int b = 0; b < 8; b++)
                {
                    if (board[a][b].type == 'K' && board[a][b].possible && board[a][b].isWhite)
                    {
                        cout << "Black checked white" << endl;
                        status = 3;
                    }
                    board[a][b].isSelected = false;
                    board[a][b].possible = false;
                }
            }
        }
    }
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j].type == 'K' && board[i][j].isWhite)
            {
                wKingAlive = true;
            }
            else if (board[i][j].type == 'K' && !board[i][j].isWhite)
            {
                bKingAlive = true;
            }
        }
    }
    if (wKingAlive && bKingAlive)
    {
        status = 1;
    }
    else if (wKingAlive && !bKingAlive)
    {
        status = 4;
        cout << "White won" << endl;
    }
    else if (!wKingAlive && bKingAlive)
    {
        status = 5;
        cout << "Black won" << endl;
    }
    }
    return status;

}