#pragma once
#ifndef APP_H_
#define APP_H_
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <filesystem>
#include "SDL.h"
#include "SDL_image.h"
#include "renderable.h"
#include "player.h"
#include "wall.h"
#include "monster/monster.h"
#include "monster/update.h"

using namespace std;

struct Room {
	unsigned int width = 0;
	unsigned int height = 0;
	vector<int> data;
	bool dir[4] = {0, 0, 0, 0};
};

class App {
public:
	App(void);
	~App();
	void Update();
	void Event();
	void Render();
	bool Running() const;
	void LoadRoom(const char* path);
	void LoadTexture(const char* path);

	void AddWall(const size_t& index, const int& x, const int& y);
	bool AddRoom(const unsigned int& index, const int& x, const int& y);
	
	
private:
	SDL_Window* window_ = nullptr;
	SDL_Renderer* renderer_ = nullptr;
	const char* title_;
	int xpos_;
	int ypos_;
	int width_;
	int height_;
	bool fullscreen_;
	bool running_;
	vector<SDL_Texture*> textures_;
	vector<SDL_Rect> textures_data_;
	vector<Room> room_data_;
	Player* player_ = nullptr;
	vector<Renderable*> to_render_;
	vector<Wall*> walls_;
	vector<Monster*> monsters_;

	Uint64 now_ = 0;
	Uint64 last_ = 0;

	bool up_ = false;
	bool down_ = false;
	bool left_ = false;
	bool right_ = false;

	double camera_x_ = 0.0;
	double camera_y_ = 0.0;

	unsigned int room_width_ = 2048;
	unsigned int room_height_ = 2048;
	unsigned size_ = 0;
	bool* tiles_ = nullptr;
	bool* path_tiles_ = nullptr;

	size_t fps_desired_ = 60;
	double second_timer_ = 0.0;
};
#endif