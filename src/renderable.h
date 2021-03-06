#pragma once
#ifndef RENDERABLE_H_
#define RENDERABLE_H_
#include "SDL.h"
#include "SDL_image.h"

struct Texture {
	SDL_Texture* texture = nullptr;
	SDL_Rect texture_data;
	SDL_Rect sprite_data;
	unsigned int subimages = 0;
};

class Renderable {
public:
	Renderable(const Texture& texture);
	~Renderable();
	void Render(SDL_Renderer* renderer, double x, double y) const;
	void RectPos(const int& x, const int& y);
	void GetRect(int& w, int& h) const;
	void SetFrame(const double& frame);
	void AddFrame(const double& frame);
	void SetImageSpeed(const double& speed);
	Renderable* Self();
	SDL_Rect ReturnRect() const { return rect_; }
	void SetTexture(const Texture& texture);
	void SetAngle(const double& angle);
	double GetFrames() const;
	void SetVParent(const int& parent) { parent_ = parent; }
	int GetVParent() const { return parent_; }
	void SetActive(const bool& a) { active_ = a; };
	bool GetActive() const { return active_; };
private:
	SDL_Texture* texture_;
	SDL_Rect rect_;
	SDL_Rect sprite_rect_;
	double subimages_ = 0;
	double subimage_ = 0;
	double image_speed_ = 1;
	double angle_ = 0.0;
	bool active_ = true;
	int parent_ = -1;
};
#endif