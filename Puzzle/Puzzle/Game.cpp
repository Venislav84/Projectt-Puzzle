#include "Game.h"
#include <iostream>
#include <string>
#include <time.h> 

bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "SDL running\n";
		isMainPageShowing = true;
		isPuzzlePageShowing = false;
		currentSelectedRightPuzzleBox;
		currentSelectedRightPuzzleBox.isSelected = false;
		xMatrixRandom = 3;
		yMatrixRandom = 2;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window != 0) //window init success
		{
			std::cout << "Window created\n";
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer != 0) //renderer init success
			{
				std::cout << "Renderer created\n";
				SDL_SetRenderDrawColor(renderer, 230, 10, 60, 255);
				loadMainPictures(xMatrixRandom, yMatrixRandom);
				drawDynamicSquares(xMatrixRandom, yMatrixRandom, width, height, 0, 0, "main_");
				SDL_RenderPresent(renderer); //"present" the drawings from the buffer to the renderer
			}
			else {
				std::cout << "renderer init failed\n";
				return false;
			}
		}
		else {
			std::cout << "window init failed\n";
			return false;
		}
	}
	else {
		std::cout << "SDL init fail\n";
		return false;
	}
	std::cout << "Initialization successful\n";
	running = true;
	return true;
}

// enumerator to handle window rectangle for readability
enum Quadrant { TL = 0, TM = 1, TR = 2, BL = 3, BM = 4, BR = 5, UNDEFINED = 100 };

Quadrant mouseDownIn = UNDEFINED; // indicates which quadrant the L or R mouse button was pressed down
Quadrant mouseDownOut = UNDEFINED; // indicates which quadrant the L mouse was released up

void Game::drawBox(string key, float boxStartX, float boxStartY, float boxWidth, float boxHeight) {
	if (currentSelectedRightPuzzleBox.isSelected == true && currentSelectedRightPuzzleBox.x == boxStartX && currentSelectedRightPuzzleBox.y == boxStartY) {
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	}
	else {
		SDL_SetRenderDrawColor(renderer, 230, 10, 60, 255);
	}
	SDL_RenderDrawLine(renderer, boxStartX, boxStartY, boxStartX + boxWidth, boxStartY); // top line
	SDL_RenderDrawLine(renderer, boxStartX + boxWidth, boxStartY, boxStartX + boxWidth, boxStartY + boxHeight); // right line
	SDL_RenderDrawLine(renderer, boxStartX + boxWidth, boxStartY + boxHeight, boxStartX, boxStartY + boxHeight); // bottom line
	SDL_RenderDrawLine(renderer, boxStartX, boxStartY, boxStartX, boxStartY + boxHeight); // left linecin
}

void Game::drawDynamicSquares(int xMatrix, int yMatrix, float ww, float wh, float startX, float startY, string keyPrefix) {
	float boxWidth = ww / xMatrix;
	float boxHeight = wh / yMatrix;
	float boxX = wh / yMatrix;
	float boxY = wh / yMatrix;
	int counter = 0;
	for (int i = 0; i < yMatrix; i++) {
		for (int j = 0; j < xMatrix; j++) {
			string key = keyPrefix + to_string(i) + to_string(j);
			if (j == 0 && i == 0) {
				boxX = startX;
				boxY = startY;
			}
			else if (j > 0 && i == 0) {
				boxX = startX + ((ww / xMatrix) * j);
				boxY = startY;
			}
			else if (j == 0 && i > 0) {
				boxX = startX;
				boxY = startY + ((wh / yMatrix) * i);
			}
			else {
				boxX = startX + ((ww / xMatrix) * j);
				boxY = startY + ((wh / yMatrix) * i);
			}

			if (key.rfind("right_puzzle", 0) == 0) {
				SDL_Rect rcTo = { boxX, boxY, boxWidth, boxHeight };
				SDL_Rect rcFrom = { positionsLeftPuzzle[cells[i][j]].x, positionsLeftPuzzle[cells[i][j]].y, positionsLeftPuzzle[cells[i][j]].w, positionsLeftPuzzle[cells[i][j]].h };
				SDL_RenderCopy(renderer, TextureManager::Instance()->getTexture("right_selected_00"), &rcFrom, &rcTo);
				positionsRightPuzzle[counter].x = rcTo.x;
				positionsRightPuzzle[counter].y = rcTo.y;
				positionsRightPuzzle[counter].w = rcTo.w;
				positionsRightPuzzle[counter].h = rcTo.h;
				positionsRightPuzzle[counter].xSrc = rcFrom.x;
				positionsRightPuzzle[counter].ySrc = rcFrom.y;
				positionsRightPuzzle[counter].wSrc = rcFrom.w;
				positionsRightPuzzle[counter].hSrc = rcFrom.h;
				counter++;
			}
			else {
				if (selectedLeftPuzzleBoxIsMatched && boxX == currentSelectedRightPuzzleBox.xSrc && boxY == currentSelectedRightPuzzleBox.ySrc) {
					SDL_Rect rcTo = { currentSelectedRightPuzzleBox.xSrc, currentSelectedRightPuzzleBox.ySrc, currentSelectedRightPuzzleBox.wSrc, currentSelectedRightPuzzleBox.hSrc };
					SDL_Rect rcFrom = { currentSelectedRightPuzzleBox.x, currentSelectedRightPuzzleBox.y, currentSelectedRightPuzzleBox.w, currentSelectedRightPuzzleBox.h };
					SDL_RenderCopy(renderer, TextureManager::Instance()->getTexture("left_selected_00"), &rcFrom, &rcTo);
				}
				else {
					TextureManager::Instance()->drawTexture(key, boxX, boxY, boxWidth, boxHeight, renderer); //renders only if visible
				}
				
				if (counter < xMatrix * yMatrix) {
					positionsLeftPuzzle[counter].x = boxX;
					positionsLeftPuzzle[counter].y = boxY;
					positionsLeftPuzzle[counter].w = boxWidth;
					positionsLeftPuzzle[counter].h = boxHeight;
					positionsLeftPuzzle[counter].key = key;
					counter++;
				}
			}
			drawBox(key, boxX, boxY, boxWidth, boxHeight);
		}
	}
}

string Game::getSelectedBox(int mouseX, int mouseY) {
	string key = "";
	int boxesLength = xMatrixRandom * yMatrixRandom;

	for (int i = 0; i < boxesLength; i++) {
		if (mouseX >= positionsLeftPuzzle[i].x && mouseX <= positionsLeftPuzzle[i].x + positionsLeftPuzzle[i].w) { // mouseX is between left x Position and the right x postion
			if (mouseY >= positionsLeftPuzzle[i].y && mouseY <= positionsLeftPuzzle[i].y + positionsLeftPuzzle[i].h) { // mouseY is between top y Position and the bottom y postion
				return positionsLeftPuzzle[i].key;
			}
		}
	}

	return key;
}

void Game::generateMatrix() {
	// initial count 5
	int matrixCount = 5;
	srand(time(0));
	int rannum = rand();


	int randomNumber = (rannum % matrixCount) + 1; // rand is from 0 to random number

	switch (randomNumber) {
	case 1: xMatrixRandom = 4; yMatrixRandom = 3; break;
	case 2: xMatrixRandom = 4; yMatrixRandom = 4; break;
	case 3: xMatrixRandom = 5; yMatrixRandom = 4; break;
	case 4: xMatrixRandom = 5; yMatrixRandom = 5; break;
	case 5: xMatrixRandom = 6; yMatrixRandom = 5; break;
	default: xMatrixRandom = 4; yMatrixRandom = 4;
	}
}

void Game::render() {

}

void Game::drawSelectedMainPicture(string key) {
	int ww, wh;
	SDL_GetWindowSize(window, &ww, &wh); //get window's width and height	

	float oneSqueareWidth = ww / (xMatrixRandom * 2);
	float oneSqueareHeight = wh / yMatrixRandom;
	float puzzleWidth = (oneSqueareWidth * xMatrixRandom) - oneSqueareWidth;
	float puzzleHeight = (oneSqueareHeight * yMatrixRandom) - oneSqueareHeight;
	float startX = oneSqueareWidth / 2;
	float startY = oneSqueareHeight / 2;

	TextureManager::Instance()->renderTextureCustom(key, renderer, startX, startY, puzzleWidth, puzzleHeight);
	TextureManager::Instance()->setSelectedMainPicturesFirst(key);
}

void Game::hideShowMainPictures(bool showPictures, int xMatrix, int yMatrix, string selectedKey) {

	for (int i = 0; i < yMatrix; i++) {
		for (int j = 0; j < xMatrix; j++) {
			string key = "main_" + to_string(i) + to_string(j);
			if (key != selectedKey) {
				TextureManager::Instance()->hideShowTexture(key, false);
			}
		}
	}

}

void Game::removeMainPictures() {

	for (int i = 0; i < yMatrixRandom; i++) {
		for (int j = 0; j < xMatrixRandom; j++) {
			string key = "main_" + to_string(i) + to_string(j);
			TextureManager::Instance()->removeTexture(key);
		}
	}

}

void Game::loadMainPictures(int xMatrix, int yMatrix) {

	int picturesLength = xMatrix * yMatrix;
	int counter = 0;

	for (int i = 0; i < yMatrix; i++) {
		for (int j = 0; j < xMatrix; j++) {
			if (counter < picturesLength) {
				string key = "main_" + to_string(i) + to_string(j);
				string imageName = "assets/00" + to_string(counter++) + ".jpg";
				TextureManager::Instance()->loadTexture(imageName.c_str(), key, renderer);
			}
		}
	}
}


void Game::randomize() {
	size_t i, j;
	int num = 0;

	for (i = 0; i < yMatrixRandom; ++i)
		for (j = 0; j < xMatrixRandom; ++j)
			cells[i][j] = -1;
	do {
		i = rnd() % yMatrixRandom;
		j = rnd() % xMatrixRandom;
		if (cells[i][j] == -1) {
			cells[i][j] = num;
			++num;
		}
	} while (num < yMatrixRandom * xMatrixRandom);
}

void Game::handleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: running = false; break; // on close window 

		case SDL_MOUSEBUTTONDOWN: {
			SDL_SetRenderDrawColor(renderer, 172, 172, 172, 255); //set drawing color
			SDL_RenderClear(renderer); // clears the previous content and paints fills the background with color
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);

			if (isMainPageShowing == true) {
				string key = getSelectedBox(mouseX, mouseY);
				drawSelectedMainPicture(key);
				generateMatrix();
				randomize();
				generatePuzzle();

			}
			else if (isPuzzlePageShowing) {
				selectedLeftPuzzleBoxIsMatched = false;
				if (!currentSelectedRightPuzzleBox.isSelected) {
					if (checkIfRightPuzzleBoxIsSelected()) {
						// Efekt na migane
					}
					else {
						clearSelectedBox();
					}
				}
				else if(checkIfLeftPuzzleBoxIsSelectedAndCompare()){
					// Efekt na na migane

					clearSelectedBox();
				}
				else {
					//if (checkIfRightPuzzleBoxIsSelected()) {
						// Efekt na migane
					//}
					//clearSelectedBox();
				}
				// 
				generatePuzzle();

			}

		}; break;
		default: break;
		}
	}
}

bool Game::checkIfRightPuzzleBoxIsSelected() {
	int mouseX, mouseY, ww, wh;
	//int button;
	SDL_GetWindowSize(window, &ww, &wh); //get window's width and height
	SDL_GetMouseState(&mouseX, &mouseY);

	int boxesLength = xMatrixRandom * yMatrixRandom;

	for (int i = 0; i < boxesLength; i++) {
		if (mouseX >= positionsRightPuzzle[i].x && mouseX <= positionsRightPuzzle[i].x + positionsRightPuzzle[i].w) { // mouseX is between left x Position and the right x postion
			if (mouseY >= positionsRightPuzzle[i].y && mouseY <= positionsRightPuzzle[i].y + positionsRightPuzzle[i].h) { // mouseY is between top y Position and the bottom y postion
				currentSelectedRightPuzzleBox.x = positionsRightPuzzle[i].x;
				currentSelectedRightPuzzleBox.y = positionsRightPuzzle[i].y;
				currentSelectedRightPuzzleBox.w = positionsRightPuzzle[i].w;
				currentSelectedRightPuzzleBox.h = positionsRightPuzzle[i].h;
				currentSelectedRightPuzzleBox.xSrc = positionsRightPuzzle[i].xSrc;
				currentSelectedRightPuzzleBox.ySrc = positionsRightPuzzle[i].ySrc;
				currentSelectedRightPuzzleBox.wSrc = positionsRightPuzzle[i].wSrc;
				currentSelectedRightPuzzleBox.hSrc = positionsRightPuzzle[i].hSrc;
				currentSelectedRightPuzzleBox.isSelected = true;
				return true;
			}
		}
	}

	return false;
}
bool Game::checkIfLeftPuzzleBoxIsSelectedAndCompare() {
	int mouseX, mouseY, ww, wh;
	//int button;
	SDL_GetWindowSize(window, &ww, &wh); //get window's width and height
	SDL_GetMouseState(&mouseX, &mouseY);

	int boxesLength = xMatrixRandom * yMatrixRandom;

	for (int i = 0; i < boxesLength; i++) {
		if (mouseX >= positionsLeftPuzzle[i].x && mouseX <= positionsLeftPuzzle[i].x + positionsLeftPuzzle[i].w) { // mouseX is between left x Position and the right x postion
			if (mouseY >= positionsLeftPuzzle[i].y && mouseY <= positionsLeftPuzzle[i].y + positionsLeftPuzzle[i].h) { // mouseY is between top y Position and the bottom y postion
				if (positionsLeftPuzzle[i].x == currentSelectedRightPuzzleBox.x && positionsLeftPuzzle[i].y == currentSelectedRightPuzzleBox.y) {
					// Kopirane
					return true;
				}
				selectedLeftPuzzleBoxIsMatched = true;
				
				return false;
			}
		}
	}

	return false;
}

void Game::clearSelectedBox() {
	currentSelectedRightPuzzleBox.x = 0;
	currentSelectedRightPuzzleBox.y = 0;
	currentSelectedRightPuzzleBox.w = 0;
	currentSelectedRightPuzzleBox.h = 0;
	currentSelectedRightPuzzleBox.xSrc = 0;
	currentSelectedRightPuzzleBox.ySrc = 0;
	currentSelectedRightPuzzleBox.wSrc = 0;
	currentSelectedRightPuzzleBox.hSrc = 0;
	currentSelectedRightPuzzleBox.isSelected = false;
}

void Game::generatePuzzle() {
	int mouseX, mouseY, ww, wh;
	//int button;
	SDL_GetWindowSize(window, &ww, &wh); //get window's width and height
	SDL_GetMouseState(&mouseX, &mouseY);

	isMainPageShowing = false;
	isPuzzlePageShowing = true;

	float oneSqueareWidth = ww / (xMatrixRandom * 2);
	float oneSqueareHeight = wh / yMatrixRandom;
	float puzzleWidth = (oneSqueareWidth * xMatrixRandom) - oneSqueareWidth;
	float puzzleHeight = (oneSqueareHeight * yMatrixRandom) - oneSqueareHeight;
	float startX = oneSqueareWidth / 2;
	float startY = oneSqueareHeight / 2;
	SDL_SetRenderDrawColor(renderer, 230, 10, 60, 255);

	// drawDynamicSquares(1, 1, puzzleWidth, puzzleHeight, startX, startY, "left_selected_");

	SDL_RenderPresent(renderer); //"present" the drawings from the buffer to the renderer

	startX = (oneSqueareWidth * xMatrixRandom) + (oneSqueareWidth / 2);
	drawDynamicSquares(1, 1, puzzleWidth, puzzleHeight, startX, startY, "right_selected_");

	startX = oneSqueareWidth / 2;

	// draw left side
	drawDynamicSquares(xMatrixRandom, yMatrixRandom, puzzleWidth, puzzleHeight, startX, startY, "left_puzzle");

	// draw right side
	startX = (oneSqueareWidth * xMatrixRandom) + (oneSqueareWidth / 2);
	drawDynamicSquares(xMatrixRandom, yMatrixRandom, puzzleWidth, puzzleHeight, startX, startY, "right_puzzle");

	SDL_RenderPresent(renderer); //"present" the drawings from the buffer to the renderer

	TextureManager::Instance()->toggleVisibility("left_selected_00");

	// draw a vertical blue line
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	SDL_RenderDrawLine(renderer, ww / 2, 0, ww / 2, wh);
	SDL_RenderPresent(renderer); //"present" the drawings from the buffer to the renderer
}

void Game::update() {
	// std::cout << "Ticks" << SDL_GetTicks()<< "\n";
	// std::cout << "Ticks / 100 " << int((SDL_GetTicks()/100)) << "\n";
	// std::cout << "(Ticks / 100 )% 5 " << int(((SDL_GetTicks()/100)%5)) << "\n\n";

	//currentFrame = int(((SDL_GetTicks() / 100) % 5)); //animate sprites %x x frames per row; /y controls the speed
}
void Game::clean() {
	std::cout << "cleaning game\n";
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
bool Game::isRunning() {
	return Game::running;
}
Game::Game() {
	Game::window = NULL;
	Game::renderer = NULL;
	Game::running = true;
	Game::currentFrame = 0; //frames start form 0
	Game::currentRow = 1; //rows start from 1
}
Game::~Game() {
}