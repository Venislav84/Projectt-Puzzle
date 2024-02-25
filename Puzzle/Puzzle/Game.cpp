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
		SDL_SetWindowResizable(window, SDL_FALSE);
		if (window != 0) //window init success
		{
			std::cout << "Window created\n";
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer != 0) //renderer init success
			{
				std::cout << "Renderer created\n";
				drawInitScreen();
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

// show main screen
void Game::drawInitScreen() {
	clearData();
	int ww, wh;
	
	SDL_GetWindowSize(window, &ww, &wh); //get window's width and height
	SDL_SetRenderDrawColor(renderer, 230, 10, 60, 255);
	loadMainPictures(xMatrixRandom, yMatrixRandom);
	drawDynamicSquares(xMatrixRandom, yMatrixRandom, ww, wh, 0, 0, 0, 0, "main_", 0);
	SDL_RenderPresent(renderer); //"present" the drawings from the buffer to the renderer
}

// initializes the data again
void Game::clearData() {
	int ww, wh;
	
	isMainPageShowing = true;
	isPuzzlePageShowing = false;
	currentSelectedRightPuzzleBox.isSelected = false;
	xMatrixRandom = 3;
	yMatrixRandom = 2;
	positionsLeftPuzzle;
	for (int i = 0; i < 60; i++) {
		positionsLeftPuzzle[i].x = 0;
		positionsLeftPuzzle[i].y = 0;
		positionsLeftPuzzle[i].w = 0;
		positionsLeftPuzzle[i].h = 0;
		positionsLeftPuzzle[i].key = "";
		positionsRightPuzzle[i].x = 0;
		positionsRightPuzzle[i].y = 0;
		positionsRightPuzzle[i].w = 0;
		positionsRightPuzzle[i].h = 0;
		positionsRightPuzzle[i].xSrc = 0;
		positionsRightPuzzle[i].ySrc = 0;
		positionsRightPuzzle[i].wSrc = 0;
		positionsRightPuzzle[i].hSrc = 0;
		positionsSelectedPicture[i].x = 0;
		positionsSelectedPicture[i].y = 0;
		positionsSelectedPicture[i].w = 0;
		positionsSelectedPicture[i].h = 0;
	}
	for (int i = 0; i < 30; i++) {
		matchedBoxes[i].left.x = 0;
		matchedBoxes[i].left.y = 0;
		matchedBoxes[i].left.w = 0;
		matchedBoxes[i].left.h = 0;
		matchedBoxes[i].right.x = 0;
		matchedBoxes[i].right.y = 0;
		matchedBoxes[i].right.w = 0;
		matchedBoxes[i].right.h = 0;
	}
}

// draws red lines around the square
//  draws blue lines around the selected square on the right side
void Game::drawBox(string key, float boxStartX, float boxStartY, float boxWidth, float boxHeight) {
	if (currentSelectedRightPuzzleBox.isSelected == true && currentSelectedRightPuzzleBox.x == boxStartX && currentSelectedRightPuzzleBox.y == boxStartY) {
		int blueLineChange = 3; // draws the blue border inward
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
		SDL_RenderDrawLine(renderer, boxStartX + blueLineChange, boxStartY + blueLineChange, boxStartX + boxWidth - blueLineChange, boxStartY + blueLineChange); // top line
		SDL_RenderDrawLine(renderer, boxStartX + boxWidth - blueLineChange, boxStartY + blueLineChange, boxStartX + boxWidth - blueLineChange, boxStartY + boxHeight - blueLineChange); // right line
		SDL_RenderDrawLine(renderer, boxStartX + boxWidth - blueLineChange, boxStartY + boxHeight - blueLineChange, boxStartX + blueLineChange, boxStartY + boxHeight - blueLineChange); // bottom line
		SDL_RenderDrawLine(renderer, boxStartX + blueLineChange, boxStartY + blueLineChange, boxStartX + blueLineChange, boxStartY + boxHeight - blueLineChange); // left line
	}

	SDL_SetRenderDrawColor(renderer, 230, 10, 60, 255);
	SDL_RenderDrawLine(renderer, boxStartX, boxStartY, boxStartX + boxWidth, boxStartY); // top line
	SDL_RenderDrawLine(renderer, boxStartX + boxWidth, boxStartY, boxStartX + boxWidth, boxStartY + boxHeight); // right line
	SDL_RenderDrawLine(renderer, boxStartX + boxWidth, boxStartY + boxHeight, boxStartX, boxStartY + boxHeight); // bottom line
	SDL_RenderDrawLine(renderer, boxStartX, boxStartY, boxStartX, boxStartY + boxHeight); // left line
}

// draw the squares on the fragmented photo or draw the main screen
void Game::drawDynamicSquares(int xMatrix, int yMatrix, float ww, float wh, float picWidth, float picHeight, float startX, float startY, string keyPrefix, float oneBoxWidth) {
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
				TextureManager::Instance()->drawOneFrameFromTexture("right_selected_00", (startX - (oneBoxWidth / 2)) + positionsLeftPuzzle[cells[i][j]].x + 5, positionsLeftPuzzle[cells[i][j]].y + 5, picWidth, picHeight, boxWidth - 10, boxHeight - 10, i + 1, j, renderer, SDL_FLIP_NONE);
				positionsRightPuzzle[counter].x = (startX - (oneBoxWidth / 2)) + positionsLeftPuzzle[cells[i][j]].x;
				positionsRightPuzzle[counter].y = positionsLeftPuzzle[cells[i][j]].y;
				positionsRightPuzzle[counter].w = boxWidth;
				positionsRightPuzzle[counter].h = boxHeight;
				positionsRightPuzzle[counter].xSrc = positionsLeftPuzzle[counter].x;
				positionsRightPuzzle[counter].ySrc = positionsLeftPuzzle[counter].y;
				positionsRightPuzzle[counter].wSrc = positionsLeftPuzzle[counter].w;
				positionsRightPuzzle[counter].hSrc = positionsLeftPuzzle[counter].h;
				counter++; // increment index of array
			}
			else if (key.rfind("left_puzzle", 0) == 0) {
				// if there is a match, show the photo in the left part
				if (checkBoxIsMatched(true, boxX, boxY)) {
					TextureManager::Instance()->drawOneFrameFromTexture("left_selected_00", boxX, boxY, picWidth, picHeight, boxWidth, boxHeight, i + 1, j, renderer, SDL_FLIP_NONE); //renders only if visible
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
			else {
				TextureManager::Instance()->drawTexture(key, boxX, boxY, boxWidth, boxHeight, renderer);

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

// hide the picture in the right rectangle if it matches витх the picture and place in the left rectangle
void Game::drawDynamicSquaresFillMatched(int xMatrix, int yMatrix, float ww, float wh, float picWidth, float picHeight, float startX, float startY, string keyPrefix, float oneBoxWidth) {
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
				if (checkBoxIsMatched(false, boxX, boxY)) {
					int blueLineChange = 3;
					SDL_SetRenderDrawColor(renderer, 172, 172, 172, 255); SDL_Rect fillRect = { boxX + blueLineChange, boxY + blueLineChange, boxWidth - (2 * blueLineChange), boxHeight - (2 * blueLineChange) };
					SDL_RenderDrawRect(renderer, &fillRect);
					SDL_RenderFillRect(renderer, &fillRect);
				}
			}
		}
	}
}

// check if the puzzle is ordered
bool Game::checkIfGameIsCompleted() {
	if (isMainPageShowing == true) {
		return false;
	}
	bool found = true; // it is always ordered until proven otherwise
	for (int i = 0; i < xMatrixRandom * yMatrixRandom; i++) {
		if (matchedBoxes[i].left.x == 0 && matchedBoxes[i].left.y == 0) {
			found = false;
		}
	}
	return found;
}

// if a match is found save them
void Game::addMatchedBox(SDL_FRect left, SDL_FRect right) {
	for (int i = 0; i < 30; i++) {
		if (matchedBoxes[i].left.x == 0 && matchedBoxes[i].left.y == 0) {
			matchedBoxes[i].left = left;
			matchedBoxes[i].right = right;
			break;
		}
	}
}

// check for a match on the left side and the right side to hide
bool Game::checkBoxIsMatched(bool isLeftBox, float x, float y) {
	for (int i = 0; i < xMatrixRandom * yMatrixRandom; i++) {
		if (isLeftBox && matchedBoxes[i].left.x == x && matchedBoxes[i].left.y == y) {
			return true;
		}
		else if (!isLeftBox && matchedBoxes[i].right.x == x && matchedBoxes[i].right.y == y) {
			return true;
		}
	}
	return false;
}

// get the image size
SDL_Point Game::getsize(SDL_Texture* texture) {
	SDL_Point size;
	SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
	return size;
}

// take the selected image from the main page
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

// create the size of the puzzle
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
	default: xMatrixRandom = 3; yMatrixRandom = 3;
	}
}

void Game::render() {
	if (checkIfGameIsCompleted()) {
		showCongratulationScreen();
	}
}

// show the congratulation screen and after 6 seconds show the main screen again
void Game::showCongratulationScreen() {
	int mouseX, mouseY, ww, wh;
	
	SDL_GetWindowSize(window, &ww, &wh); //get window's width and height
	SDL_RenderClear(renderer);
	TextureManager::Instance()->loadTexture("assets/Congratulations.jpg", "congrats", renderer);
	TextureManager::Instance()->drawTexture("congrats", 0, 0, ww, wh, renderer, SDL_FLIP_NONE);
	SDL_RenderPresent(renderer);
	SDL_Delay(6000);

	drawInitScreen();
}
// the selected image places it in the selected texture
void Game::drawSelectedMainPicture(string key) {
	TextureManager::Instance()->setSelectedMainPicturesFirst(key);
}

// load the images on the main screen
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

// shuffle the puzzle pieces in the right side
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
		case SDL_WINDOWEVENT_RESIZED:
			generatePuzzle(); break; 

		case SDL_MOUSEBUTTONDOWN: {
			SDL_SetRenderDrawColor(renderer, 172, 172, 172, 255); //set drawing color
			SDL_RenderClear(renderer); // clears the previous content and paints fills the background with color
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);

			// when the main screen is displayed
			if (isMainPageShowing == true) {
				string key = getSelectedBox(mouseX, mouseY);
				drawSelectedMainPicture(key);
				generateMatrix();
				randomize();

			}
			else if (isPuzzlePageShowing) { // when the puzzle is displayed
				selectedLeftPuzzleBoxIsMatched = false;
				if (!currentSelectedRightPuzzleBox.isSelected) {
					if (!checkIfRightPuzzleBoxIsSelected()) {
						clearSelectedBox();
					}
				}
				else if (checkIfLeftPuzzleBoxIsSelectedAndCompare()) {
					clearSelectedBox();
				}
				else {
					if (!checkIfRightPuzzleBoxIsSelected()) {
						clearSelectedBox();
					}
				}

			}
			generatePuzzle();
		}; break;
		default: break;
		}
	}
}
// checks if a right puzzle piece is selected
bool Game::checkIfRightPuzzleBoxIsSelected() {
	int mouseX, mouseY, ww, wh;
	
	SDL_GetWindowSize(window, &ww, &wh); //get window's width and height
	SDL_GetMouseState(&mouseX, &mouseY);

	int boxesLength = xMatrixRandom * yMatrixRandom;

	for (int i = 0; i < boxesLength; i++) {
		if (mouseX >= positionsRightPuzzle[i].x && mouseX <= positionsRightPuzzle[i].x + positionsRightPuzzle[i].w
			&& mouseY >= positionsRightPuzzle[i].y && mouseY <= positionsRightPuzzle[i].y + positionsRightPuzzle[i].h) { // mouseY is between top y Position and the bottom y postion
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

	return false;
}
// checks if a piece of the left puzzle is selected and compares it to a piece of the right puzzle if it is selected
bool Game::checkIfLeftPuzzleBoxIsSelectedAndCompare() {
	int mouseX, mouseY, ww, wh;
	SDL_GetWindowSize(window, &ww, &wh); 
	SDL_GetMouseState(&mouseX, &mouseY);

	int boxesLength = xMatrixRandom * yMatrixRandom;

	for (int i = 0; i < boxesLength; i++) {
		if (mouseX >= positionsLeftPuzzle[i].x && mouseX <= positionsLeftPuzzle[i].x + positionsLeftPuzzle[i].w) { 
			if (mouseY >= positionsLeftPuzzle[i].y && mouseY <= positionsLeftPuzzle[i].y + positionsLeftPuzzle[i].h) { 
				if (positionsLeftPuzzle[i].x == currentSelectedRightPuzzleBox.xSrc && positionsLeftPuzzle[i].y == currentSelectedRightPuzzleBox.ySrc) {
					selectedLeftPuzzleBoxIsMatched = true;
					SDL_FRect left = { positionsLeftPuzzle[i].x, positionsLeftPuzzle[i].y, positionsLeftPuzzle[i].w, positionsLeftPuzzle[i].h };
					SDL_FRect right = { currentSelectedRightPuzzleBox.x, currentSelectedRightPuzzleBox.y, currentSelectedRightPuzzleBox.w, currentSelectedRightPuzzleBox.h };
					addMatchedBox(left, right);
					return true;
				}
				return false;
			}
		}
	}

	return false;
}

// clears the data for the selected box
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

// create puzzle
void Game::generatePuzzle() {
	int mouseX, mouseY, ww, wh;
	//int button;
	SDL_GetWindowSize(window, &ww, &wh); //get window's width and height
	SDL_GetMouseState(&mouseX, &mouseY);

	isMainPageShowing = false;
	isPuzzlePageShowing = true;

	SDL_Point selectedPicture = getsize(TextureManager::Instance()->getTexture("left_selected_00"));

	float pictureWidth = selectedPicture.x / xMatrixRandom;
	float pictureHeight = selectedPicture.y / yMatrixRandom;
	float oneSqueareWidth = ww / (xMatrixRandom * 2);
	float oneSqueareHeight = wh / yMatrixRandom;
	float puzzleWidth = (oneSqueareWidth * xMatrixRandom) - oneSqueareWidth;
	float puzzleHeight = (oneSqueareHeight * yMatrixRandom) - oneSqueareHeight;
	float startX = oneSqueareWidth / 2;
	float startY = oneSqueareHeight / 2;
	SDL_SetRenderDrawColor(renderer, 230, 10, 60, 255);

	startX = oneSqueareWidth / 2;

	// draw left side
	drawDynamicSquares(xMatrixRandom, yMatrixRandom, puzzleWidth, puzzleHeight, pictureWidth, pictureHeight, startX, startY, "left_puzzle", oneSqueareWidth);

	// draw right side
	startX = (oneSqueareWidth * xMatrixRandom) + (oneSqueareWidth / 2);
	drawDynamicSquares(xMatrixRandom, yMatrixRandom, puzzleWidth, puzzleHeight, pictureWidth, pictureHeight, startX, startY, "right_puzzle", oneSqueareWidth);

	SDL_RenderPresent(renderer); //"present" the drawings from the buffer to the renderer
	// hide the finished squares in the right puzzle
	drawDynamicSquaresFillMatched(xMatrixRandom, yMatrixRandom, puzzleWidth, puzzleHeight, pictureWidth, pictureHeight, startX, startY, "right_puzzle", oneSqueareWidth);
	// draw a vertical blue line
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	SDL_RenderDrawLine(renderer, ww / 2, 0, ww / 2, wh);
	SDL_RenderPresent(renderer); //"present" the drawings from the buffer to the renderer
}

void Game::update() {

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