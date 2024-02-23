#include "Game.h"
#include <iostream>
#include <string>
#include <time.h> 

bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "SDL running\n";
		isMainPageShowing = true;
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
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				loadMainPictures(xMatrixRandom, yMatrixRandom);
				drawDynamicSquares(xMatrixRandom, yMatrixRandom, width, height, 0, 0);
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
//enum Quadrant { TL = 0, TM = 1, TR = 2, BL = 3, BM = 4, BR = 5, UNDEFINED = 100 };
//
//Quadrant mouseDownIn = UNDEFINED; // indicates which quadrant the L or R mouse button was pressed down
//Quadrant mouseDownOut = UNDEFINED; // indicates which quadrant the L mouse was released up

//// Texture positions for the moustache texture
//int texPosX, texPosY = 0;

// texture dimensions;  down-scaling not implemented - must be the size of the image or higher
//int w = 100; int h = 60;

void Game::drawBox(string key, float boxStartX, float boxStartY, float boxWidth, float boxHeight) {
	TextureManager::Instance()->drawTexture(key, boxStartX, boxStartY, boxWidth, boxHeight, renderer); //renders only if visible
	SDL_RenderDrawLine(renderer, boxStartX, boxStartY, boxStartX + boxWidth, boxStartY); // top line
	SDL_RenderDrawLine(renderer, boxStartX + boxWidth, boxStartY, boxStartX + boxWidth, boxStartY + boxHeight); // right line
	SDL_RenderDrawLine(renderer, boxStartX + boxWidth, boxStartY + boxHeight, boxStartX, boxStartY + boxHeight); // bottom line
	SDL_RenderDrawLine(renderer, boxStartX, boxStartY, boxStartX, boxStartY + boxHeight); // left line
}

void Game::drawDynamicSquares(int xMatrix, int yMatrix, float ww, float wh, float startX, float startY) {
	float boxWidth = ww / xMatrix;
	float boxHeight = wh / yMatrix;
	float boxX = wh / yMatrix;
	float boxY = wh / yMatrix;
	int counter = 0;
	for (int i = 0; i < yMatrix; i++) {
		for (int j = 0; j < xMatrix; j++) {
			string key = to_string(i) + to_string(j);
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
			drawBox(key, boxX, boxY, boxWidth, boxHeight);
			if (counter < xMatrix * yMatrix) {
				positions[counter].x = boxX;
				positions[counter].y = boxY;
				positions[counter].w = boxWidth;
				positions[counter].h = boxHeight;
				positions[counter].key = key;
				counter++;
			}
		}
	}
}

string Game::getSelectedBox(int mouseX, int mouseY) {
	string key = "";
	int boxesLength = xMatrixRandom * yMatrixRandom;

	for (int i = 0; i < boxesLength; i++) {
		if (mouseX >= positions[i].x && mouseX <= positions[i].x + positions[i].w) { // mouseX is between left x Position and the right x postion
			if (mouseY >= positions[i].y && mouseY <= positions[i].y + positions[i].h) { // mouseY is between top y Position and the bottom y postion
				return positions[i].key;
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
	SDL_SetRenderDrawColor(renderer, 172, 172, 172, 255); //set drawing color
	SDL_RenderClear(renderer); // clears the previous content and paints fills the background with color

	int ww, wh;
	SDL_GetWindowSize(window, &ww, &wh); //get window's width and height	

	SDL_SetRenderDrawColor(renderer, 230, 10, 60, 255);

	if (isMainPageShowing == true) {
		drawDynamicSquares(xMatrixRandom, yMatrixRandom, ww, wh, 0, 0);
	}
	else {
		// 


		float oneSqueareWidth = ww / (xMatrixRandom * 2);
		float oneSqueareHeight = wh / yMatrixRandom;
		float puzzleWidth = (oneSqueareWidth * xMatrixRandom) - oneSqueareWidth;
		float puzzleHeight = (oneSqueareHeight * yMatrixRandom) - oneSqueareHeight;
		float startX = oneSqueareWidth / 2;
		float startY = oneSqueareHeight / 2;

		drawDynamicSquares(1, 1, puzzleWidth, puzzleHeight, startX, startY);

		startX = (oneSqueareWidth * xMatrixRandom) + (oneSqueareWidth / 2);
		drawDynamicSquares(1, 1, puzzleWidth, puzzleHeight, startX, startY);

		startX = oneSqueareWidth / 2;

		// draw left side
		drawDynamicSquares(xMatrixRandom, yMatrixRandom, puzzleWidth, puzzleHeight, startX, startY);

		// draw right side
		startX = (oneSqueareWidth * xMatrixRandom) + (oneSqueareWidth / 2);
		drawDynamicSquares(xMatrixRandom, yMatrixRandom, puzzleWidth, puzzleHeight, startX, startY);

		// draw a vertical blue line
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
		SDL_RenderDrawLine(renderer, ww / 2, 0, ww / 2, wh);
	}

	SDL_RenderPresent(renderer); //"present" the drawings from the buffer to the renderer
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
			string key = to_string(i) + to_string(j);
			if (key != selectedKey) {
				TextureManager::Instance()->hideShowTexture(key, false);
			}
		}
	}

}

void Game::loadMainPictures(int xMatrix, int yMatrix) {

	int picturesLength = xMatrix * yMatrix;
	int counter = 0;

	for (int i = 0; i < yMatrix; i++) {
		for (int j = 0; j < xMatrix; j++) {
			if (counter < picturesLength) {
				string key = to_string(i) + to_string(j);
				string imageName = "assets/00" + to_string(counter++) + ".jpg";
				TextureManager::Instance()->loadTexture(imageName.c_str(), key, renderer);
			}
		}
	}
}

void Game::handleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		int mouseX, mouseY, ww, wh;
		//int button;
		SDL_GetWindowSize(window, &ww, &wh); //get window's width and height
		switch (event.type) {
		case SDL_QUIT: running = false; break; // on close window 

		case SDL_MOUSEBUTTONDOWN: {

			if (isMainPageShowing == true) {
				/*Get rectangle*/
				SDL_GetMouseState(&mouseX, &mouseY);

				if (event.button.button == SDL_BUTTON_LEFT /*|| event.button.button == SDL_BUTTON_RIGHT*/ ) {


					if (isMainPageShowing == true) {
						string key = getSelectedBox(mouseX, mouseY);
						drawSelectedMainPicture(key);
					}
				}

 

				isMainPageShowing = false;
				generateMatrix();
			}
			else {
				TextureManager::Instance()->hideShowTexture("", false);
			}
		}; break;

			//		

			//	case SDL_MOUSEBUTTONUP: {
			//		/*Get quadrant on mouse up - to ensure the mouse stayed in the same quadrant*/
			//		SDL_GetMouseState(&mouseX, &mouseY);// get mouse position

			//		if (event.button.button == SDL_BUTTON_LEFT) {
			//			switch (((mouseX > ww / 3) + (mouseY > wh / 2) * 2)) {
			//			case 0: {
			//				mouseDownOut = TL;
			//				if (mouseDownIn == mouseDownOut) {
			//					TextureManager::Instance()->toggleVisibility("TL"); //toggle visibility
			//				}
			//			}; break;
			//			case 1: {
			//				mouseDownOut = TM;
			//				if (mouseDownIn == mouseDownOut) {
			//					TextureManager::Instance()->toggleVisibility("TM"); //toggle visibility
			//				}
			//			} break;
			//			case 2: {
			//				mouseDownOut = TR;
			//				if (mouseDownIn == mouseDownOut) {
			//					TextureManager::Instance()->toggleVisibility("TR"); //toggle visibility
			//				}
			//			} break;
			//			case 3: {
			//				mouseDownOut = BL;
			//				if (mouseDownIn == mouseDownOut) {
			//					TextureManager::Instance()->toggleVisibility("BL"); //toggle visibility
			//				}
			//			} break;
			//			case 4: {
			//				mouseDownOut = BM;
			//				if (mouseDownIn == mouseDownOut) {
			//					TextureManager::Instance()->toggleVisibility("BM"); //toggle visibility
			//				}
			//			} break;
			//			case 5: {
			//				mouseDownOut = BR;
			//				if (mouseDownIn == mouseDownOut) {
			//					TextureManager::Instance()->toggleVisibility("BR"); //toggle visibility
			//				}
			//			} break;
			//			default: break;
			//			}
			//		}
			//		//if (event.button.button == SDL_BUTTON_RIGHT) {
			//		//	// update global variables using references
			//		//	int& tPosX = texPosX; int& tPosY = texPosY;
			//		//	tPosX = mouseX - w / 3;
			//		//	tPosY = mouseY - h / 2;
			//		//}
			//	}; break;
			//	//case SDL_KEYDOWN: {
			//	//	// update global variables using references
			//	//	int& tPosX = texPosX; int& tPosY = texPosY;
			//	//	if (event.key.keysym.sym == SDLK_UP)
			//	//		tPosY -= 5;
			//	//	if (event.key.keysym.sym == SDLK_DOWN)
			//	//		tPosY += 5;
			//	//	if (event.key.keysym.sym == SDLK_LEFT)
			//	//		tPosX -= 5;
			//	//	if (event.key.keysym.sym == SDLK_RIGHT)
			//	//		tPosX += 5;
			//	//} break;
		default: break;
		}
	}
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