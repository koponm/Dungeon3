#pragma once
#ifndef RENDERABLE_H_
#define RENDERABLE_H_
#include "SDL.h"
#include "SDL_image.h"

class Renderable {
public:
	Renderable(SDL_Texture* texture, SDL_Rect rect);
	~Renderable();
	void Render(SDL_Renderer* renderer, double x, double y) const;
	void RectPos(int x, int y);
	void GetRect(int& w, int& h);
private:
	SDL_Texture* texture_;
	SDL_Rect rect_;
};
#endif