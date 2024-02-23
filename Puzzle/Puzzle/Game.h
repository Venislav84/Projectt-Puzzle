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

class Game {
public:
    Game();
    ~Game();
    bool init(const char* title, int xpos, int ypos, int width, int height, int flags);
    void render();
    void update();
    void handleEvents();
    void clean();
    void hideShowMainPictures(bool showPictures, int xMatrix, int yMatrix, string selectedKey);
    bool isRunning();
    void generateMatrix();
    void drawDynamicSquares(int xMatrix, int yMatrix, float ww, float wh, float startX, float startY);
    void loadMainPictures(int xMatrix, int yMatrix);
    void drawBox(string key, float boxStartX, float boxStartY, float boxWidth, float boxHeight);
    string getSelectedBox(int mouseX, int mouseY);
    void drawSelectedMainPicture(string key);

private:
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    bool running;
    bool isMainPageShowing;

    int currentFrame;
    int currentRow;
    int xMatrixRandom;
    int yMatrixRandom;
    SDL_FRectWithKey        positions[30];
    std::random_device rnd;
    SDL_Surface* img = nullptr;
    SDL_Texture* tex = nullptr;
    int cells[6][5];
};
