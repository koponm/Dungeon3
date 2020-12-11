#pragma once
#ifndef APP_H_
#define APP_H_
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <string>
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
#include "projectile/projectile.h"
#include "projectile/fireball.h"
#include "projectile/iceball.h"
#include "projectile/arrow.h"
#include "projectile/melee.h"
#include "monster/initializeMonster.h"
#include "monster/bossPhases.h"
#include "monster/update.h"
#include "items.h"
#include "chest.h"
#include "hud.h"
#include "floor.h"
#include "door.h"
#include "ladder.h"
#include "buyable.h"

using namespace std;

struct Room {
	unsigned int width = 0;
	unsigned int height = 0;
	vector<int> data;
	bool dir[4] = {0, 0, 0, 0};
};

struct DungeonRoom {
	int index = 0;
	bool dir[4] = { 0, 0, 0, 0 };
	bool door[4] = { 0, 0, 0, 0 };
	vector<int> monster_spawns;
};

class App {
public:
	App(void);
	~App();
	void Update();
	void Event();
	void Render();
	bool Running() const;
	void PlayerCastsProjectile(int previous_x, int previous_y);
	void LoadRoom(const char* path);
	void AddProjectile(TextureType type, const int& x, const int& y,double speed,double dir, shared_ptr<Renderable> parent, ProjectileType pro);
	std::shared_ptr<Wall> AddWall(TextureType type, const int& x, const int& y);
	void AddFloor(TextureType type, const int& x, const int& y);
	std::shared_ptr<Door> AddDoor(TextureType type, const int& x, const int& y);
	void AddChest(TextureType type, const int& x, const int& y);
	bool AddRoom(const unsigned int& index, const int& x, const int& y);
	void Generate();
	void RenderText(const char* text, TTF_Font* font, SDL_Color color, int x, int y);
	void RenderBar(int x, int y, int w, int h, double max_value, double value, SDL_Color FGColor, SDL_Color BGColor);
	SDL_Color color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void LoadSound(const char* path);
	void PlaySound(const unsigned& index, const int& loops);
	void Recursive(const unsigned int& index, std::shared_ptr<Door> pointer, const unsigned int& previous_dir);
	void Reset();
	void MainMenu();
	void LoadRooms();
	void BossBattle();
	
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
	shared_ptr<TextureHandler> textures_ = nullptr;
	vector<Room> room_data_;
	shared_ptr<Player> player_ = nullptr;
	list<shared_ptr<Renderable>> to_render_;
	list<shared_ptr<Renderable>> entities_;

	vector<shared_ptr<Wall>> walls_;
	vector<shared_ptr<Monster>> monsters_;
	vector<shared_ptr<Projectile>> projectiles_;
	vector<shared_ptr<Item>> items_;
	vector<shared_ptr<Chest>> chests_;
	vector<shared_ptr<Floor>> floor_;
	vector<shared_ptr<Door>> doors_;
	vector<shared_ptr<HUD_object>> hud_;

	vector<shared_ptr<Ladder>> ladders_;
	vector<shared_ptr<Buyable>> buyables_;

	vector<Mix_Chunk*> sounds_;

	vector<int> door_spots_;

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
	bool m1_ = false;
	bool m1_released_ = true;

	bool main_menu_ = true;
	bool boss_stage_ = false;

	double camera_x_ = 0.0;
	double camera_y_ = 0.0;
	double shoot_timer_ = 0.0;

	double damage_m_ = 1.0;
	double speed_m_ = 1.0;
	shared_ptr<BossHandler> bossptr = nullptr;
	unsigned int boss_phase_ = 0;


	bool noclip_ = false;
	bool fullvis_ = false;

	unsigned int room_width_ = 1440;
	unsigned int room_height_ = 1440;
	unsigned size_ = 0;
	bool* tiles_ = nullptr;
	bool* path_tiles_ = nullptr;
	bool* hidden_ = nullptr;
	bool* visible_ = nullptr;
	DungeonRoom* room_ = nullptr;
	unsigned int dungeon_width_ = 0;
	unsigned int dungeon_height_ = 0;
	float difficulty_ = 0;
	float difficulty_mult_ = 1;
	unsigned int stage_ = 0;
	int last_dir_ = 0;
	bool next_level_ = false;
	bool death_ = false;
	size_t fps_desired_ = 60;
	double second_timer_ = 0.0;
	double delta_time_ = 0.0;
	double tick_timer_ = 0.0;
	int mouse_x_ = 0;
	int mouse_y_ = 0;
	double mouse_player_angle_ = 0;
};

#endif // APP_H_
