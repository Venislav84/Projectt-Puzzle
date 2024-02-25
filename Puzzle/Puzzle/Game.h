#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include "TextureManager.h"
#include <string>
#include <random>

using namespace std;

typedef struct SDL_FRectWithKey
{
    float x;
    float y;
    float w;
    float h;
    string key;
} SDL_FRectWithKey;
typedef struct SDL_FRectWithKeyAndSrc
{
    float x;
    float y;
    float w;
    float h;
    float xSrc;
    float ySrc;
    float wSrc;
    float hSrc;
    string key;
    bool isSelected;
} SDL_FRectWithKeyAndSrc;

typedef struct SDL_FRectMatched
{
    SDL_FRect left;
    SDL_FRect right;
} SDL_FRectMatched;

class Game {
public:
    Game();
    ~Game();
    bool init(const char* title, int xpos, int ypos, int width, int height, int flags);
    void render();
    void update();
    void handleEvents();
    void clean();
    bool isRunning();
    void generateMatrix();
    void drawDynamicSquares(int xMatrix, int yMatrix, float ww, float wh, float picWidth, float picHeight, float startX, float startY, string keyPrefix, float oneBoxWidth);
    void drawDynamicSquaresFillMatched(int xMatrix, int yMatrix, float ww, float wh, float picWidth, float picHeight, float startX, float startY, string keyPrefix, float oneBoxWidth);
    void loadMainPictures(int xMatrix, int yMatrix);
    void drawBox(string key, float boxStartX, float boxStartY, float boxWidth, float boxHeight);
    string getSelectedBox(int mouseX, int mouseY);
    void drawSelectedMainPicture(string key);
    void randomize();
    void generatePuzzle();
    bool checkIfRightPuzzleBoxIsSelected();
    bool checkIfLeftPuzzleBoxIsSelectedAndCompare();
    void clearSelectedBox();
    SDL_Point getsize(SDL_Texture* texture);
    bool checkBoxIsMatched(bool isLeftBox, float x, float y);
    void addMatchedBox(SDL_FRect left, SDL_FRect right);
    bool checkIfGameIsCompleted();
    void drawInitScreen();
    void showCongratulationScreen();
    void clearData();

private:
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    bool running;
    bool isMainPageShowing;
    bool isPuzzlePageShowing;
    SDL_FRectWithKeyAndSrc currentSelectedRightPuzzleBox;
    bool selectedLeftPuzzleBoxIsMatched;

    int currentFrame;
    int currentRow;
    int xMatrixRandom;
    int yMatrixRandom;
    SDL_FRectWithKey        positionsLeftPuzzle[60];
    SDL_FRectWithKeyAndSrc  positionsRightPuzzle[60];
    SDL_FRect       positionsSelectedPicture[60];
    SDL_FRectMatched       matchedBoxes[30];
    std::random_device rnd;
    SDL_Surface* img = nullptr;
    SDL_Texture* tex = nullptr;
    int cells[6][5];
};