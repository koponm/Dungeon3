#pragma once
#ifndef APP_H_
#define APP_H_
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <filesystem>
#include <time.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "renderable.h"
#include "constants.h"
#include "textureHandler.h"
#include "player.h"
#include "wall.h"
#include "projectile.h"
#include "fireball.h"
#include "iceball.h"
#include "melee.h"
#include "monster/initializeMonster.h"
#include "monster/update.h"
#include "items.h"
#include "chest.h"

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
	void AddProjectile(TextureType type, const int& x, const int& y,double speed,double dir, Renderable* parent, ProjectileType pro);
	void AddWall(TextureType type, const int& x, const int& y);
	void AddItem(const int& x, const int& y, ItemType type = ItemType::random);
	void AddChest(TextureType type, const int& x, const int& y);
	bool AddRoom(const unsigned int& index, const int& x, const int& y);
	void Generate();
	void RenderText(const char* text, TTF_Font* font, SDL_Color color, int x, int y);
	void LoadSound(const char* path);
	void PlaySound(const unsigned& index, const int& loops);
	
private:
	SDL_Window* window_ = nullptr;
	SDL_Renderer* renderer_ = nullptr;
	TTF_Font* default_font_ = nullptr;
	const char* title_;
	int xpos_;
	int ypos_;
	int width_;
	int height_;
	bool fullscreen_;
	bool running_;
	TextureHandler* textures_ = nullptr;
	vector<Room> room_data_;
	Player* player_ = nullptr;
	vector<Renderable*> to_render_;
	vector<Wall*> walls_;
	vector<Monster*> monsters_;
	vector<Projectile*> projectiles_;
	vector<Item*> items_;
	vector<Chest*> chests_;

	vector<Mix_Chunk*> sounds_;

	double now_ = 0;
	double last_ = 0;

	bool up_ = false;
	bool down_ = false;
	bool left_ = false;
	bool right_ = false;
	bool f_ = false;
	bool space_ = false;
	bool one_ = false;
	bool two_ = false;
	bool three_ = false;
	bool m1_ = false;
	bool m1_released_ = true;

	double camera_x_ = 0.0;
	double camera_y_ = 0.0;

	unsigned int room_width_ = 1024;
	unsigned int room_height_ = 1024;
	unsigned size_ = 0;
	bool* tiles_ = nullptr;
	bool* path_tiles_ = nullptr;
	unsigned int difficulty_ = 1;

	size_t fps_desired_ = 60;
	double second_timer_ = 0.0;
	double delta_time_ = 0.0;
	double tick_timer_ = 0.0;
	int mouse_x_ = 0;
	int mouse_y_ = 0;
	double mouse_player_angle_ = 0;
};
#endif