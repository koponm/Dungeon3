#pragma once
#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include "SDL.h"
#include "SDL_image.h"
#include "constants.h"
#include "renderable.h"

#include <type_traits>
#include <vector>
#include <string>

class TextureHandler {
public:
	TextureHandler(SDL_Renderer* renderer);
	~TextureHandler() {}

	const Texture& Get(TextureType type) const;
	void LoadTexture(const char* path);
private:
	std::vector<Texture> textures_;
	SDL_Renderer* renderer_ = nullptr;
};

#endif // TEXTURE_HANDLER_H