#include "../headers/board.h"

using namespace std;

void Board::setupBoard()
{
    int empties = 16;

    for (int i = 0; i < 8;)
    {
        board[1][i] = {'P', 8 + i, false};
        board[6][i] = {'P', 48 + i, true};
        i++;
    }

    for (int i = 2; i < 6; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[i][j] = {'E', empties++, true};
        }
    }

    board[0][0] = {'R', 0, false};
    board[0][7] = {'R', 7, false};
    board[7][0] = {'R', 56, true};
    board[7][7] = {'R', 63, true};

    board[0][1] = {'N', 1, false};
    board[0][6] = {'N', 6, false};
    board[7][1] = {'N', 57, true};
    board[7][6] = {'N', 62, true};

    board[0][2] = {'B', 2, false};
    board[0][5] = {'B', 5, false};
    board[7][2] = {'B', 58, true};
    board[7][5] = {'B', 61, true};

    board[0][3] = {'Q', 3, false};
    board[7][3] = {'Q', 59, true};

    board[0][4] = {'K', 4, false};
    board[7][4] = {'K', 60, true};
    board[4][3] = {'B', 35, true};
}

bool Board::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return false;
    }

    gWindow = SDL_CreateWindow("Mick's Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
                SDL_SetRenderDrawColor(gRenderer, 200, 200, 200, 255);
            }
            if (board[i][j].isSelected && (i + j) % 2 == 0)
            {
                SDL_SetRenderDrawColor(gRenderer, 75, 75, 75, 255);
            }
            else if (board[i][j].isSelected && (i + j) % 2 != 0)
            {
                SDL_SetRenderDrawColor(gRenderer, 150, 150, 150, 255);
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
                    board[i][j].isWhite == true ? board[i - 1][j].possible = true : board[i + 1][j].possible = true;
                    board[i][j].isWhite == true ? board[i - 2][j].possible = true : board[i + 2][j].possible = true;
                    break;
                case 'R':
                    for (int index = i - 1; index >= 0; index--)
                    {
                        if (board[index][j].type != 'E')
                        {
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
                            break;
                        }
                        else
                        {
                            board[i][index].possible = true;
                        }
                    }
                    break;
                case 'B':
                    for (int indexI = i - 1; indexI >= 0; indexI--)
                    {
                        for (int indexJ = j - 1; indexJ >= 0; indexJ--)
                        {
                            if (board[indexI][indexJ].type != 'E')
                            {
                                break;
                            }
                            else
                            {
                                board[indexI][indexJ].possible = true;
                            }
                        }
                    }
                    break;
                default:
                    cout << "No piece" << endl;
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
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[i][j].isSelected = false;
            board[i][j].possible = false;
        }
    }
    board[selectedY][selectedX].isSelected = true;
}