#include "renderable.h"

Renderable::Renderable(SDL_Texture* texture, SDL_Rect rect) {
	texture_ = texture;
	rect_ = rect;
}

Renderable::~Renderable() {
}

void Renderable::Render(SDL_Renderer* renderer, double x, double y) const {
	SDL_Rect temp = rect_;
	temp.x -= x;
	temp.y -= y;
	SDL_RenderCopy(renderer, texture_, NULL, &temp);
}

void Renderable::RectPos(int x, int y) {
	rect_.x = x;
	rect_.y = y;
}

void Renderable::GetRect(int& w, int& h) {
	w = rect_.w;
	h = rect_.w;
}