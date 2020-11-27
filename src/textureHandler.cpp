#include "textureHandler.h"

TextureHandler::TextureHandler(SDL_Renderer* renderer)
	: renderer_(renderer)
{
	LoadTexture("assets/player_8.png");
	LoadTexture("assets/wall2_1.png");
	LoadTexture("assets/fire_3.png");
	LoadTexture("assets/monster_1.png");
	LoadTexture("assets/potion1_1.png");
	LoadTexture("assets/potion2_1.png");
	LoadTexture("assets/chest_2.png");
	LoadTexture("assets/ghost_2.png");
	LoadTexture("assets/skeleton_2.png");
	LoadTexture("assets/staff_1.png");
	LoadTexture("assets/sword_1.png");

	LoadTexture("assets/ladderdown_1.png");
	LoadTexture("assets/ladderup_1.png");
	LoadTexture("assets/doorh_1.png");
	LoadTexture("assets/doorv_1.png");
	LoadTexture("assets/skeletonmage_2.png");
	LoadTexture("assets/skeletonarcher_2.png");
	LoadTexture("assets/fireball_5.png");
}

const Texture& TextureHandler::Get(TextureType type) const {
	return textures_[type];
}

void TextureHandler::LoadTexture(const char* path) {
	Texture temp_texture;

	SDL_Surface* temp = IMG_Load(path);

	SDL_Rect temp_rect;
	temp_rect.w = temp->w;
	temp_rect.h = temp->h;
	temp_rect.x = 0;
	temp_rect.y = 0;

	temp_texture.texture = SDL_CreateTextureFromSurface(renderer_, temp);

	std::string name = path;
	std::string images;
	char t;
	unsigned i = 0;

	while (true) {
		t = path[name.length() - 5 - i];
		if (t != '_') {
			images += t;
			i++;
		}
		else {
			break;
		}
	}

	i = stoi(images, nullptr, 10);

	temp_rect.w = temp_rect.w / i;
	temp_texture.texture_data = temp_rect;
	temp_texture.sprite_data = temp_rect;
	temp_texture.subimages = i;

	textures_.push_back(temp_texture);

	SDL_FreeSurface(temp);
}