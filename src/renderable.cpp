#include "renderable.h"

Renderable::Renderable(const Texture& texture) {
	texture_ = texture.texture;
	rect_ = texture.texture_data;
	sprite_rect_ = texture.sprite_data;
	subimages_ = texture.subimages;
}

Renderable::~Renderable() {
}

void Renderable::Render(SDL_Renderer* renderer, double x, double y) const {
	SDL_Rect temp = rect_;
	temp.x -= x;
	temp.y -= y;
	SDL_Rect temp_sprite = sprite_rect_;
	temp_sprite.x = subimage_ * temp_sprite.w;
	SDL_RenderCopy(renderer, texture_, &temp_sprite, &temp);
}

void Renderable::RectPos(int x, int y) {
	rect_.x = x;
	rect_.y = y;
}

void Renderable::GetRect(int& w, int& h) {
	w = sprite_rect_.w;
	h = sprite_rect_.h;
}