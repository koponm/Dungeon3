#include "renderable.h"
#include <iostream>

Renderable::Renderable(const Texture& texture) {
	texture_ = texture.texture;
	rect_ = texture.texture_data;
	sprite_rect_ = texture.sprite_data;
	subimages_ = texture.subimages;
	image_speed_ = 1.0 / subimages_;
}

Renderable::~Renderable() {
}

Renderable* Renderable::Self() {
	return this;
}

void Renderable::Render(SDL_Renderer* renderer, double x, double y) const {
	SDL_Rect temp = rect_;
	temp.x -= x;
	temp.y -= y;
	SDL_Rect temp_sprite = sprite_rect_;
	temp_sprite.x = floor(subimage_) * temp_sprite.w;
	SDL_RenderCopy(renderer, texture_, &temp_sprite, &temp);
}

void Renderable::RectPos(const int& x, const int& y) {
	rect_.x = x;
	rect_.y = y;
}

void Renderable::GetRect(int& w, int& h) const {
	w = sprite_rect_.w;
	h = sprite_rect_.h;
}

void Renderable::SetFrame(const double& frame) {
	subimage_ = fmod(frame, subimages_);
}

void Renderable::AddFrame(const double& frame) {
	subimage_ += frame * image_speed_;
	subimage_ = fmod(subimage_, subimages_);
}

void Renderable::SetImageSpeed(const double& speed) {
	image_speed_ = speed;
}

void Renderable::SetTexture(const Texture& texture) {
	texture_ = texture.texture;
	rect_ = texture.texture_data;
	sprite_rect_ = texture.sprite_data;
	subimages_ = texture.subimages;
	image_speed_ = 1.0 / subimages_;
	subimage_ = 0;
}
