#pragma once
#include "Game.h"
#include <string>
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rect.h>
#include <SDL_render.h>

class TextureManager {
public:
	bool loadTexture(const char* fileName, std::string id, SDL_Renderer* ren);
	bool hideShowTexture(std::string id, bool hideShow);

	void drawTexture(std::string id,
		int x, int y,
		int width, int height,
		SDL_Renderer* ren,
		SDL_RendererFlip flip = SDL_FLIP_NONE,
		int rotation = 0);

	void drawOneFrameFromTexture(std::string id,
		int x, int y,
		int width, int height,
		int currentRow, int currentFrame,
		SDL_Renderer* ren,
		SDL_RendererFlip flip = SDL_FLIP_NONE);

	void toggleVisibility(std::string id);

	void renderTextureCustom(std::string key, SDL_Renderer* ren, int x, int y, int w, int h);
	void removeTexture(std::string id);
	void setSelectedMainPicturesFirst(std::string id);
	SDL_Texture* getTexture(std::string key);

	static TextureManager* Instance();

private:
	TextureManager() {}
	static TextureManager* instance;
	std::map<std::string, SDL_Texture*> textureMap; //map to store the loaded textures with loadTexture
	std::map<std::string, bool> display; //map to store instruction for displaying the loaded textures
};
