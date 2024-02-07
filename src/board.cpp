#include "../headers/board.h"

using namespace std;

void Board::setupBoard()
{
    gameStatus = 1;
    whiteTurn = true;
    sprintf(score, "Current score W-B: %2d - %2d", wScore, bScore);
    sprintf(gameInfo, "It's %s turn.", whiteTurn ? "White's" : "Black's");
    sprintf(gameCheck, " ");
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

    board[7][2] = {'E', true};
    board[7][3] = {'E', true};
}

bool Board::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return false;
    }
    if (TTF_Init() == -1)
    {
        cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
        return false;
    }

    gWindow = SDL_CreateWindow("Mick's chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
    TTF_Font *font = TTF_OpenFont("../assets/27733932592.ttf", 12);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // Black color
    SDL_RenderClear(gRenderer);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            SDL_Rect squareRect = {j * SQUARE_SIZE_WIDTH, i * SQUARE_SIZE_HEIGHT, SQUARE_SIZE_WIDTH, SQUARE_SIZE_HEIGHT};
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
            if (board[i][j].castleSquare && (i + j) % 2 == 0 && board[selectedPiece[0]][selectedPiece[1]].type == 'K')
            {
                SDL_SetRenderDrawColor(gRenderer, 75, 150, 75, 255);
            }
            else if (board[i][j].castleSquare && (i + j) % 2 != 0 && board[selectedPiece[0]][selectedPiece[1]].type == 'K')
            {
                SDL_SetRenderDrawColor(gRenderer, 115, 150, 115, 255);
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
                SDL_Rect pieceRect = {j * SQUARE_SIZE_WIDTH, i * SQUARE_SIZE_HEIGHT, SQUARE_SIZE_WIDTH, SQUARE_SIZE_HEIGHT};
                SDL_RenderCopy(gRenderer, gPieceTextures[texture], NULL, &pieceRect);
            }
        }
    }
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *debugTextSurface = TTF_RenderText_Solid(font, score, textColor);
    SDL_Texture *debugTextTexture = SDL_CreateTextureFromSurface(gRenderer, debugTextSurface);
    SDL_Rect debugTextRect = {SCREEN_WIDTH - debugTextSurface->w - 23, (SCREEN_HEIGHT / 2) - 10, debugTextSurface->w, debugTextSurface->h};
    SDL_RenderCopy(gRenderer, debugTextTexture, NULL, &debugTextRect);
    SDL_FreeSurface(debugTextSurface);
    SDL_DestroyTexture(debugTextTexture);

    SDL_Surface *additionalInfoSurface = TTF_RenderText_Solid(font, gameInfo, textColor);
    SDL_Texture *additionalInfoTexture = SDL_CreateTextureFromSurface(gRenderer, additionalInfoSurface);
    SDL_Rect additionalInfoRect = {SCREEN_WIDTH - additionalInfoSurface->w - 50, (SCREEN_HEIGHT / 2) + 5, additionalInfoSurface->w, additionalInfoSurface->h};
    SDL_RenderCopy(gRenderer, additionalInfoTexture, NULL, &additionalInfoRect);
    SDL_FreeSurface(additionalInfoSurface);
    SDL_DestroyTexture(additionalInfoTexture);

    SDL_Surface *gameCheckSurface = TTF_RenderText_Solid(font, gameCheck, textColor);
    SDL_Texture *gameCheckTexture = SDL_CreateTextureFromSurface(gRenderer, gameCheckSurface);
    SDL_Rect gameCheckRect = {SCREEN_WIDTH - gameCheckSurface->w - 50, (SCREEN_HEIGHT / 2) + 20, gameCheckSurface->w, gameCheckSurface->h};
    SDL_RenderCopy(gRenderer, gameCheckTexture, NULL, &gameCheckRect);
    SDL_FreeSurface(gameCheckSurface);
    SDL_DestroyTexture(gameCheckTexture);

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
                        if (board[i - 1][j].type == 'E' && board[i - 2][j].type == 'E' && !board[i][j].hasMoved)
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
                        if (board[i + 1][j].type == 'E' && board[i + 2][j].type == 'E' && !board[i][j].hasMoved)
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
                    if (!board[i][j].hasMoved)
                    {
                        for (int index = j - 1; index >= 0; index--)
                        {
                            if (board[i][index].type == 'E')
                            {
                                board[i][index].castlePath = true;
                            }
                            else
                            {
                                if (board[i][index].type == 'R' && !board[i][index].hasMoved)
                                {
                                    board[i][index].castleSquare = true;
                                }
                                break;
                            }
                        }
                        for (int index = j + 1; index < 8; index++)
                        {
                            if (board[i][index].type == 'E')
                            {
                                board[i][index].castlePath = true;
                            }
                            else
                            {
                                if (board[i][index].type == 'R' && !board[i][index].hasMoved)
                                {
                                    board[i][index].castleSquare = true;
                                }
                                break;
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
    selectedX = X / SQUARE_SIZE_WIDTH;
    selectedY = Y / SQUARE_SIZE_HEIGHT;
    if (board[selectedY][selectedX].type != 'E' && whiteTurn == board[selectedY][selectedX].isWhite && !pieceSelected)
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
        selectedPiece[0] = selectedY;
        selectedPiece[1] = selectedX;
        pieceSelected = true;
        sprintf(gameCheck, "Selected piece at %d,%d", selectedY, selectedX);
    }
    else if (board[selectedY][selectedX].type == 'E' && pieceSelected && board[selectedY][selectedX].possible || board[selectedY][selectedX].isWhite != whiteTurn && pieceSelected && board[selectedY][selectedX].possible || board[selectedY][selectedX].isWhite == whiteTurn && pieceSelected && board[selectedY][selectedX].type == 'R' && !board[selectedY][selectedX].hasMoved || board[selectedY][selectedX].isWhite == whiteTurn && pieceSelected && board[selectedY][selectedX].type != 'K' && board[selectedY][selectedX].type != 'E')
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (board[i][j].isSelected)
                {
                    if (board[selectedY][selectedX].isWhite != board[i][j].isWhite || board[selectedY][selectedX].type == 'E')
                    {
                        board[selectedY][selectedX].type = board[i][j].type;
                        board[selectedY][selectedX].isWhite = board[i][j].isWhite;
                        board[selectedY][selectedX].hasMoved = true;
                        board[i][j].type = 'E';
                        board[i][j].isWhite = true;
                        board[i][j].hasMoved = false;
                        pieceSelected = false;
                        sprintf(gameCheck, "Moved piece %d,%d to %d,%d", selectedY, selectedX, i, j);
                        whiteTurn = !whiteTurn;
                        sprintf(gameInfo, "It's %s turn.", whiteTurn ? "White's" : "Black's");
                    }
                    else if (board[selectedY][selectedX].type == 'R' && board[selectedY][selectedX].isWhite == board[i][j].isWhite && board[i][j].type == 'K')
                    {
                        sprintf(gameCheck, "Trying to castle ");
                        if (selectedY == 0 && selectedX == 0 && board[selectedY][selectedX + 1].castlePath && board[selectedY][selectedX + 2].castlePath && board[selectedY][selectedX + 3].castlePath && !board[i][j].hasMoved && !board[selectedY][selectedX].hasMoved)
                        {
                            sprintf(gameCheck, "Castling %d,%d with %d,%d", i, j, selectedY, selectedX);
                            board[selectedY][selectedX + 2].type = board[i][j].type;
                            board[selectedY][selectedX + 2].isWhite = board[i][j].isWhite;
                            board[selectedY][selectedX + 2].hasMoved = true;
                            board[selectedY][selectedX + 3].type = board[selectedY][selectedX].type;
                            board[selectedY][selectedX + 3].isWhite = board[selectedY][selectedX].isWhite;
                            board[selectedY][selectedX + 3].hasMoved = true;
                            board[i][j].type = 'E';
                            board[i][j].isWhite = true;
                            board[i][j].castleSquare = false;
                            board[selectedY][selectedX].type = 'E';
                            board[selectedY][selectedX].isWhite = true;
                            board[selectedY][selectedX].castleSquare = false;
                            pieceSelected = false;
                            whiteTurn = !whiteTurn;
                            sprintf(gameInfo, "It's %s turn.", whiteTurn ? "White's" : "Black's");
                        }

                        else if (selectedY == 0 && selectedX == 7 && board[selectedY][selectedX - 1].castlePath && board[selectedY][selectedX - 2].castlePath && !board[i][j].hasMoved && !board[selectedY][selectedX].hasMoved)
                        {
                            sprintf(gameCheck, "Castling %d,%d with %d,%d", i, j, selectedY, selectedX);
                            board[selectedY][selectedX - 1].type = board[i][j].type;
                            board[selectedY][selectedX - 1].isWhite = board[i][j].isWhite;
                            board[selectedY][selectedX - 1].hasMoved = true;
                            board[selectedY][selectedX - 2].type = board[selectedY][selectedX].type;
                            board[selectedY][selectedX - 2].isWhite = board[selectedY][selectedX].isWhite;
                            board[selectedY][selectedX - 2].hasMoved = true;
                            board[i][j].type = 'E';
                            board[i][j].isWhite = true;
                            board[i][j].castleSquare = false;
                            board[selectedY][selectedX].type = 'E';
                            board[selectedY][selectedX].isWhite = true;
                            board[selectedY][selectedX].castleSquare = false;
                            pieceSelected = false;
                            whiteTurn = !whiteTurn;
                            sprintf(gameInfo, "It's %s turn.", whiteTurn ? "White's" : "Black's");
                        }

                        else if (selectedY == 7 && selectedX == 0 && board[selectedY][selectedX + 1].castlePath && board[selectedY][selectedX + 2].castlePath && board[selectedY][selectedX + 3].castlePath && !board[i][j].hasMoved && !board[selectedY][selectedX].hasMoved)
                        {
                            sprintf(gameCheck, "Castling %d,%d with %d,%d", i, j, selectedY, selectedX);
                            board[selectedY][selectedX + 2].type = board[i][j].type;
                            board[selectedY][selectedX + 2].isWhite = board[i][j].isWhite;
                            board[selectedY][selectedX + 2].hasMoved = true;
                            board[selectedY][selectedX + 3].type = board[selectedY][selectedX].type;
                            board[selectedY][selectedX + 3].isWhite = board[selectedY][selectedX].isWhite;
                            board[selectedY][selectedX + 3].hasMoved = true;
                            board[i][j].type = 'E';
                            board[i][j].isWhite = true;
                            board[i][j].castleSquare = false;
                            board[selectedY][selectedX].type = 'E';
                            board[selectedY][selectedX].isWhite = true;
                            board[selectedY][selectedX].castleSquare = false;
                            pieceSelected = false;
                            whiteTurn = !whiteTurn;
                            sprintf(gameInfo, "It's %s turn.", whiteTurn ? "White's" : "Black's");
                        }

                        else if (selectedY == 7 && selectedX == 7 && board[selectedY][selectedX - 1].castlePath && board[selectedY][selectedX - 2].castlePath && !board[i][j].hasMoved && !board[selectedY][selectedX].hasMoved)
                        {
                            sprintf(gameCheck, "Castling %d,%d with %d,%d", i, j, selectedY, selectedX);
                            board[selectedY][selectedX - 1].type = board[i][j].type;
                            board[selectedY][selectedX - 1].isWhite = board[i][j].isWhite;
                            board[selectedY][selectedX - 1].hasMoved = true;
                            board[selectedY][selectedX - 2].type = board[selectedY][selectedX].type;
                            board[selectedY][selectedX - 2].isWhite = board[selectedY][selectedX].isWhite;
                            board[selectedY][selectedX - 2].hasMoved = true;
                            board[i][j].type = 'E';
                            board[i][j].isWhite = true;
                            board[i][j].castleSquare = false;
                            board[selectedY][selectedX].type = 'E';
                            board[selectedY][selectedX].isWhite = true;
                            board[selectedY][selectedX].castleSquare = false;
                            pieceSelected = false;
                            whiteTurn = !whiteTurn;
                            sprintf(gameInfo, "It's %s turn.", whiteTurn ? "White's" : "Black's");
                        }
                        else
                        {
                            for (int i = 0; i < 8; i++)
                            {
                                for (int j = 0; j < 8; j++)
                                {
                                    board[i][j].isSelected = false;
                                    board[i][j].possible = false;
                                    board[i][j].castlePath = false;
                                    board[i][j].castleSquare = false;
                                }
                            }
                            board[selectedY][selectedX].isSelected = true;
                            selectedPiece[0] = selectedY;
                            selectedPiece[1] = selectedX;
                            pieceSelected = true;
                            sprintf(gameCheck, "Selected piece at %d,%d", selectedY, selectedX);
                        }
                    }
                    else
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
                        selectedPiece[0] = selectedY;
                        selectedPiece[1] = selectedX;
                        pieceSelected = true;
                    }
                    for (int i = 0; i < 8; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            board[i][j].castlePath = false;
                            board[i][j].castleSquare = false;
                        }
                    }
                }
            }
        }
    }
    else if (board[selectedY][selectedX].type == 'K' && whiteTurn == board[selectedY][selectedX].isWhite && pieceSelected)
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
        selectedPiece[0] = selectedY;
        selectedPiece[1] = selectedX;
        sprintf(gameCheck, "Selected piece at %d,%d", selectedY, selectedX);
    }
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
    if (!pieceSelected)
    {
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
                            sprintf(gameCheck, "White checks Black!");
                            status = 2;
                        }
                        board[a][b].isSelected = false;
                        board[a][b].possible = false;
                        board[a][b].castlePath = false;
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
                            sprintf(gameCheck, "Black checks White!");
                            status = 3;
                        }
                        board[a][b].isSelected = false;
                        board[a][b].possible = false;
                        board[a][b].castlePath = false;
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
            wScore++;
            sprintf(score, "Current score W-B: %2d - %2d", wScore, bScore);
        }
        else if (!wKingAlive && bKingAlive)
        {
            status = 5;
            bScore++;
            sprintf(score, "Current score W-B: %2d - %2d", wScore, bScore);
        }
    }
    return status;
}

void Board::printHasMoved()
{
    cout << " " << endl;
    cout << "Has moved: " << endl;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << board[i][j].hasMoved << " ";
        }
        cout << endl;
    }
}
void Board::printType()
{
    cout << " " << endl;
    cout << "Types: " << endl;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << board[i][j].type << " ";
        }
        cout << endl;
    }
}
void Board::printSelected()
{
    cout << " " << endl;
    cout << "Selected: " << castleSelected << endl;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << board[i][j].isSelected << " ";
        }
        cout << endl;
    }
}
void Board::printCastlePath()
{
    cout << " " << endl;
    cout << "CastlePath: " << endl;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << board[i][j].castlePath << " ";
        }
        cout << endl;
    }
}