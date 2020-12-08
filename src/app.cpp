#include "app.h"

App::App(void)
:	title_	("Dungeon3"),
	xpos_	(SDL_WINDOWPOS_CENTERED),
	ypos_	(SDL_WINDOWPOS_CENTERED),
	width_	(800),
	height_	(600),
	fullscreen_ (false)
{
	if (!SDL_Init(SDL_INIT_EVERYTHING)) {
		TTF_Init();
		Mix_Init(MIX_INIT_OGG);

		window_ = SDL_CreateWindow(title_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_, (fullscreen_ ? (int) SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN));
		renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
		default_font_ = TTF_OpenFont("assets/arial.ttf", 16);
		SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
		
		// Riku: If you plan on adding more sounds, check that they're .wav files and their frequency is 44100 hz, otherwise contact me!
		// .ogg files take up a lot of system memory, check if you can find a way to stream it instead..?

		Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096);
		Mix_AllocateChannels(4);

		LoadSound("assets/spellcast.wav");
		LoadSound("assets/music.ogg");
		LoadSound("assets/pop.wav");
		LoadSound("assets/chest.wav");
		LoadSound("assets/potion.wav");
		LoadSound("assets/bow.wav");
		LoadSound("assets/swing.wav");

		PlaySound(1, -1);

		textures_ = std::shared_ptr<TextureHandler>(new TextureHandler(renderer_));

		player_ = std::shared_ptr<Player>(new Player((textures_->Get(TextureType::player)), 0, 0));

		MainMenu();


		//HUD
		hud_.emplace_back(new HUD_object(textures_->Get(hud), 735, 5, ItemType::sword, 5));
		hud_.emplace_back(new HUD_object(textures_->Get(hud), 342, 540, ItemType::health_potion, 0));
		hud_.emplace_back(new HUD_object(textures_->Get(hud), 400, 540, ItemType::mana_potion, 2));

		running_ = true;
	} else {
		running_ = false;
	}
}

void App::LoadRooms() {
	room_data_.clear();
	LoadRoom("assets/startroom_0110.txt");
	LoadRoom("assets/endroom_1111.txt");
	LoadRoom("assets/deadend_1111.txt");

	LoadRoom("assets/largeroom_1111.txt");

	LoadRoom("assets/largecorner_0011.txt");
	LoadRoom("assets/largecorner_1001.txt");
	LoadRoom("assets/largetunnel_0101.txt");
	LoadRoom("assets/largetunnel_1010.txt");
}

App::~App() {
	TTF_CloseFont(default_font_);
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
	TTF_Quit();
}

void App::Update() {

	if (next_level_) {
		bossptr = nullptr;
		if (main_menu_ || boss_stage_) {
			LoadRooms();
			srand((unsigned)time(NULL));
			main_menu_ = false;
			boss_stage_ = false;
		}
		next_level_ = false;
		difficulty_ += difficulty_mult_;
		stage_++;
		
		if (stage_ % 3 == 0) {
			boss_stage_ = true;
		}

		Reset();
		if (boss_stage_) {
			BossBattle();
		} else {
			Generate();
		}
	}

	if (death_) {
		bossptr = nullptr;
		death_ = false;
		Reset();
		entities_.clear();
		stage_ = 0;
		damage_m_ = 1.0;
		speed_m_ = 1.0;
		player_ = std::shared_ptr<Player>(new Player((textures_->Get(TextureType::player)), 0, 0));
		room_data_.clear();
		MainMenu();
		main_menu_ = true;
		boss_stage_ = false;
		difficulty_ = 0;
	}

	now_ = (double)SDL_GetTicks();
	delta_time_ = max((now_ - last_) / 1000.0f * fps_desired_, 1.0 / 1000.0f);
	second_timer_ += delta_time_ / fps_desired_;
	
	tick_timer_ += delta_time_;
	double previous_x, previous_y;
	player_ -> GetPos(previous_x, previous_y);

	// Update 2 times per second

	if (second_timer_ >= 0.5) {
		second_timer_ -= 0.5;
		monster::CalculatePath(monsters_, path_tiles_, previous_x, previous_y, size_, room_width_, room_height_);
		//
		for (auto& i : monsters_) {
			if (i->GetTimer() != 0) {
				i->SetTimer(i->GetTimer() - 0.5);
			}
		}
		player_->SetMana(std::min(player_->GetMaxMana(), player_->GetMana() + 3.0));
		player_->SetHealth(std::min(player_->GetMaxHealth(), player_->GetHealth() + 0.2 / difficulty_mult_));
	}
	
	if (shoot_timer_ > 0.0) {
		shoot_timer_ -= delta_time_ / fps_desired_;
	}

	if (f_) {//check if player and item or chest intersect
		f_ = false;
		SDL_Rect rect1 = player_->ReturnRect();
		if (!items_.empty()){
			for (auto& i : items_) {
				SDL_Rect rect2 = i->ReturnRect();
				if (SDL_HasIntersection(&rect1, &rect2)) {
					double m1, m2;
					i->GetStats(m1, m2);
					i->SetActive(false);
					std::shared_ptr<Item> last_item = player_->AddItem(i, textures_);
					if (last_item != nullptr) {
						last_item -> SetStats(damage_m_, speed_m_);
						to_render_.push_back(last_item);
						items_.push_back(last_item);
						damage_m_ = m1;
						speed_m_ = m2;
					}
					PlaySound(2, 0);
					break;
				}
			}
		}
		for (auto& i : chests_) {
			SDL_Rect rect2 = i->ReturnRect();
			if (SDL_HasIntersection(&rect1, &rect2)) {
				if (!i->ChestOpen()) {
					i->OpenChest();
					int x1, y1;
					i->GetPos(x1, y1);

					int lootn = rand() % 3 + 1;
					
					for (auto j = 0; j < lootn; j++) {
						int rx, ry;
						rx = cos(double(j) / double(lootn) * M_PI * 2.0) * 32;
						ry = sin(double(j) / double(lootn) * M_PI * 2.0) * 32;
						std::shared_ptr<Item> temp = item::GetItem(x1 + rx, y1 + ry , textures_, player_->GetWeapon(), ItemType::random, player_->GetLevel());
						if (temp != nullptr) {
							to_render_.push_back(temp);
							items_.push_back(temp);
						}
					}


					PlaySound(3, 0);
					break;
				}
			}
		}
		
		for (auto& i : ladders_) {
			SDL_Rect rect2 = i -> ReturnRect();
			if (SDL_HasIntersection(&rect1, &rect2)) {
				if (!boss_stage_) {
					next_level_ = true;
					
				} else {
					bool cleared = true;
					for (auto& m: monsters_) {
						if (!(m -> Dead())) {
							cleared = false;
						}
					}
					if (cleared) {
						next_level_ = true;
					}
					
					
				}
			}
		}

		double xx, yy;
		player_ -> GetPos(xx, yy);

		for (auto i : doors_) {
			int begin = i -> GetFirst();
			SDL_Rect rect2 = i -> ReturnRect();
			if (SDL_HasIntersection(&rect1, &rect2)) {
				PlaySound(3, 0);
				vector<int> connected = i -> GetConnected();
				for (auto& r : connected) {
					if (r != -1) {
						if (hidden_[r]) {
							int maxm = room_[r].monster_spawns.size();
							if (maxm > 0) {
								if (maxm == 1) {
									maxm = (rand() % 2 == 0) * (difficulty_mult_ > 1);
								} else {
									maxm = floor(maxm * min(int(max(difficulty_mult_ * 0.8f, 0.6f)), maxm));
								}
							}
							int count = 0;
							for (auto& m : room_[r].monster_spawns) {
								if (count < maxm) {
									int nx = (m % (room_width_ / 32)) * 32;
									int ny = (int)floor(m / (room_width_ / 32)) * 32;
									monster::AddMonster(monsters_, textures_, nx, ny, difficulty_);
									entities_.emplace_back(monsters_[monsters_.size() - 1]);
									count++;
								}
							}
						}
						hidden_[r] = false;
					}
				}
				int end = connected[connected.size() - 1];
				int tile0, tile1;

				std::shared_ptr<Wall> wall0 = nullptr;
				std::shared_ptr<Wall> wall1 = nullptr;

				for (auto j : doors_) {
					int begin2 = j -> GetFirst();
					vector<int> connected2 = j -> GetConnected();
					int end2 = connected2[connected2.size() - 1];
					if (begin2 == end && end2 == begin) {
						j -> GetTiles(tile0, tile1);
						j -> GetWalls(wall0, wall1);

						if (wall0 != nullptr && wall1 != nullptr) {
							wall0 -> SetActive(false);
							wall1 -> SetActive(false);
							walls_.erase(remove(walls_.begin(), walls_.end(), wall0), walls_.end());
							walls_.erase(remove(walls_.begin(), walls_.end(), wall1), walls_.end());
						}

						path_tiles_[tile0] = true;
						path_tiles_[tile1] = true;
						j -> SetActive(false);
						doors_.erase(remove(doors_.begin(), doors_.end(), j), doors_.end());
						break;
					}
				}
				i -> GetTiles(tile0, tile1);
				i -> GetWalls(wall0, wall1);

				if (wall0 != nullptr && wall1 != nullptr) {
					wall0 -> SetActive(false);
					wall1 -> SetActive(false);
					walls_.erase(remove(walls_.begin(), walls_.end(), wall0), walls_.end());
					walls_.erase(remove(walls_.begin(), walls_.end(), wall1), walls_.end());
				}

				path_tiles_[tile0] = true;
				path_tiles_[tile1] = true;
				i -> SetActive(false);
				doors_.erase(remove(doors_.begin(), doors_.end(), i), doors_.end());
				break;
			}
		}
		
	}
	if (one_) {
		one_ = false;
		if (player_->GetHealthPotions() > 0) {
			player_->UseHealthPotion();
			PlaySound(4, 0);
		}
	}
	if (two_) {
		two_ = false;
		if (player_->GetManaPotions() > 0) {
			player_->UseManaPotion();
			PlaySound(4, 0);
		}
	}
	if (three_) {
		three_ = false;
		// this could be used for something
		double l = 1.0 + sqrt(player_->GetLevel() / 2.0) * 0.25 / difficulty_mult_;
		player_->AddXp(25.0 * l);
	}
	if (m1_) {
		m1_ = false;
		PlayerCastsProjectile((int)previous_x, (int)previous_y);
	}

	double x1, y1;
	int w1, h1;
	player_ -> GetPos(x1, y1);
	player_ -> GetRect(w1, h1);

	if (!noclip_) {
		for (auto& i : walls_) {
			int x2, y2;
			int w2, h2;
			i -> GetPos(x2, y2);
			i -> GetRect(w2, h2);

			if (x1 < ((double) x2 + w2) && x1 + w1 > x2 && y1 < ((double) y2 + h2) && y1 + h1 > y2) {
				double x_res = (x1 - x2);
				x_res = x_res + (x_res < 0 ? w2 : -w2);

				double y_res = (y1 - y2);
				y_res = y_res + (y_res < 0 ? h2 : -h2);

				if (fabs(x_res) >= 4.0) {
					while (fabs(y_res) > 0.0) {
						player_ -> AddPos(0, -y_res);
						player_ -> SetVel(1, 0.0);

						player_ -> GetPos(x1, y1);
						y_res = (y1 - y2);
						y_res = y_res + (y_res < 0 ? h2 : -h2);
					}
				} else if (fabs(y_res) >= 4.0) {
					while (fabs(x_res) > 0.0) {
						player_ -> AddPos(-x_res, 0);
						player_ -> SetVel(0, 0.0);

						player_->GetPos(x1, y1);
						x_res = (x1 - x2);
						x_res = x_res + (x_res < 0 ? w2 : -w2);
					}
				}
			}
		}
	}
	for (auto& i : monsters_) {
		double x4, y4;
		int w4, h4;
		i->GetPos(x4, y4);
		i->GetRect(w4, h4);
		double monster_player_angle_ = fmod(540.0 - atan2f((float)(y4 - 16 - y1), (float)(x4 - 16 - x1)) * 180.0 / M_PI, 360.0);
		if (i->HasLignOfSight() && !i->IsMelee() && i->GetTimer()==0 && !i->Dead()) {
			x4 = x4 + 8.0;
			y4 = y4 + 8.0;
			if (i->GetProjectile() == ProjectileType::Arrow) {
				AddProjectile(TextureType::arrow, (int)x4, (int)y4, 250, monster_player_angle_, i, ProjectileType::Arrow);
			}
			else if (i->GetProjectile() == ProjectileType::IceBall) {
				AddProjectile(TextureType::iceball, (int)x4, (int)y4, 250, monster_player_angle_, i, i->GetProjectile());
			}
			i -> SetTimer(1.5);
			if (i->GetMonsterType() == MonsterType::necromancer) {
				i -> SetTimer(0.5);
			}
			
		}
		if (i->IsMelee() && i->GetTimer() == 0 && !i->Dead()) {
			if (x1 < ((double)x4 + w4) && x1 + w1 > x4 && y1 < ((double)y4 + h4) && y1 + h1 > y4) {
				x4 = x4 - 8.0 + 2.0 * cos(monster_player_angle_ * M_PI / 180.0);
				y4 = y4 - 8.0 - 2.0 * sin(monster_player_angle_ * M_PI / 180.0);
				AddProjectile(TextureType::melee, (int)x4, (int)y4, 0, 0, i, i->GetProjectile());
				i->SetTimer(1.0);
			}
		}
	}

	if (tick_timer_ >= 1.0) {
		double speed = player_ -> GetSpeed() / fps_desired_;

		if ((up_ || down_) && (left_ || right_)) {
			speed /= sqrt(2);
		}

		if (up_) {
			player_ -> AddVel(0, -speed);
		}
		if (down_) {
			player_ -> AddVel(0, speed);
		}
		if (left_) {
			player_ -> AddVel(-speed, 0);
		}
		if (right_) {
			player_ -> AddVel(speed, 0);
		}

		player_ -> CalcPos(fps_desired_);

		if (delta_time_ >= 1.0) {
			player_ -> SetPos(previous_x, previous_y);
		}

		for (auto& i : to_render_) {
			i -> AddFrame(4.0 / fps_desired_);
		}

		for (auto& i : entities_) {
			i->AddFrame(4.0 / fps_desired_);
		}

		player_->AddFrame(4.0 / fps_desired_);

		if (!monsters_.empty()) {
			monster::UpdateMonsters(monsters_, delta_time_, fps_desired_,
				up_ || down_ || left_ || right_,
				room_width_, walls_, textures_->Get(TextureType::tombstone), entities_);
		}

		for (auto& i : projectiles_) {
			if (i->GetActive()) {
				if (i->GetSpeed() == 0) {
					i->SetActive(false);
				}
				i -> CalcPos(fps_desired_);
				double x2, y2;
				int w2, h2;
				i->GetPos(x2, y2);
				i->GetRect(w2, h2);
				for (auto& j : walls_) {
					int x3, y3;
					int w3, h3;
					j->GetPos(x3, y3);
					j->GetRect(w3, h3);
					if (x2 < ((double)x3 + w3) && x2 + w2 > x3 && y2 < ((double)y3 + h3) && y2 + h2 > y3) {
						//i->SetVel(1, 0);
						//i->SetVel(0, 0);
						i -> SetActive(false);
					}
				}
				if(i->GetParent()==player_){
					for (auto& j : monsters_) {
						if (j->Dead()) { continue; }
						double x3, y3;
						int w3, h3;
						j->GetPos(x3, y3);
						j->GetRect(w3, h3);
						if (x2 < ((double)x3 + w3) && x2 + w2 > x3 && y2 < ((double)y3 + h3) && y2 + h2 > y3) {
							//i->SetVel(1, 0);
							//i->SetVel(0, 0);
							i->SetActive(false);
							j->SetHealth(j->GetHealth() - i->GetDamage() * damage_m_);
							if (j -> GetHealth() <= 0) {
								// + sqrt(player_ -> GetLevel() / 2.0)
								double l = 1.0 + sqrt(player_->GetLevel() / 2.0) * 0.25 / difficulty_mult_;
								player_ -> AddXp(j -> GetMaxHealth() * l);
								player_ -> AddMoney(5);
							}
						}
					}
				}
				else { //monsters projectile hit the player
					if (x2 < ((double)x1 + w1) && x2 + w2 > x1 && y2 < ((double)y1 + h1) && y2 + h2 > y1) {
						//i->SetVel(1, 0);
						//i->SetVel(0, 0);
						i->SetActive(false);
						player_->SetHealth(player_->GetHealth() - (
							i->GetDamage() * (1.0 + 0.25 * difficulty_) * difficulty_mult_
							));
					}
				}

			}

		}

		// Room "frustum culling"
		for (unsigned j = 0; j < dungeon_height_; j++) {
			for (unsigned i = 0; i < dungeon_width_; i++) {
				int x0 = i * 512, x1 = (i + 1) * 512;
				int y0 = j * 512, y1 = (j + 1) * 512;
				visible_[unsigned(j * dungeon_width_ + i)] = (x1 - camera_x_ >= 0 && x0 <= camera_x_ + width_) && (y1 - camera_y_ >= 0 && y0 <= camera_y_ + height_);
			}
		}
		while (tick_timer_ >= 1.0) {
			tick_timer_ -= 1.0;
		}
	}

	for (auto i : hud_) {
		i -> UpdateHUD(player_);
	}

	//despawn inactive items / mobs
	for (auto it = to_render_.begin(); it != to_render_.end();) {
		if (!((*it)->GetActive())) {
			it = to_render_.erase(it);
		} else {
			it++;
		}
	}

	for (auto it = entities_.begin(); it != entities_.end();) {
		if (!((*it)->GetActive())) {
			it = entities_.erase(it);
		} else {
			it++;
		}
	}

	for (auto it = items_.begin(); it != items_.end();) {
		if (!((*it)->GetActive())) {
			std::shared_ptr<Item> ptr = (*it);
			it = items_.erase(it);
		}
		else {
			it++;
		}
	}

	for (auto it = projectiles_.begin(); it != projectiles_.end();) {
		if (!((*it) -> GetActive())) {
			std::shared_ptr<Projectile> ptr = (*it);
			it = projectiles_.erase(it);
		} else {
			it++;
		}
	}

	if (player_->GetHealth() <= 0) {
		death_ = true;
	}

	if (main_menu_) {
		difficulty_mult_ = (previous_x <= 688) * 0.5 + (previous_x > 688 && previous_x < 752) * 1.0 + (previous_x >= 752) * 2;
	}

	player_ -> GetPos(camera_x_, camera_y_);

	double x_offset = width_ / 2.0;
	double y_offset = height_ / 2.0;

	camera_x_ = floor(camera_x_ - x_offset + 0.5 + w1 / 2);
	camera_y_ = floor(camera_y_ - y_offset + 0.5 + h1 / 2);

	camera_x_ = min(max(0.0, camera_x_), max(0.0, room_width_ - x_offset * 2));
	camera_y_ = min(max(0.0, camera_y_), max(0.0, room_height_ - y_offset * 2));

	mouse_player_angle_ = fmod(540.0 - atan2f((float)(y1 + 16 - camera_y_ - mouse_y_), (float)(x1 + 16 - camera_x_ - mouse_x_)) * 180.0 / M_PI, 360.0);
	
	last_ = now_;

}

void App::Event() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
		default:
			// Nothing here
		break;
		case SDL_KEYUP:
			
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				default:
				break;
				case SDLK_w:
					up_ = event.key.type == SDL_KEYDOWN ? true : false;
				break;
				case SDLK_s:
					down_ = event.key.type == SDL_KEYDOWN ? true : false;
				break;
				case SDLK_a:
					left_ = event.key.type == SDL_KEYDOWN ? true : false;
				break;
				case SDLK_d:
					right_ = event.key.type == SDL_KEYDOWN ? true : false;
				break;
				case SDLK_f:
					f_ = event.key.type == SDL_KEYDOWN ? true : false;
				break;
				case SDLK_SPACE:
					space_ = event.key.type == SDL_KEYDOWN ? true : false;
				break;
				case SDLK_1:
					one_ = event.key.type == SDL_KEYDOWN ? true : false;
					break;
				case SDLK_2:
					two_ = event.key.type == SDL_KEYDOWN ? true : false;
					break;
				case SDLK_3:
					three_ = event.key.type == SDL_KEYDOWN ? true : false;
					break;
				case SDLK_ESCAPE:
					SDL_Quit();
				break;
			}
		break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button) {
			default:
			break;
			case SDL_BUTTON_LEFT:
				// left mouse button
				if (m1_released_) {
					m1_ = event.key.type == SDL_MOUSEBUTTONDOWN ? true : false;
					m1_released_ = false;
				}
			break;
			case SDL_BUTTON_RIGHT:
				// right mouse button
			break;
			}
		break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button) {
			default:
			break;
			case SDL_BUTTON_LEFT:
				// left mouse button
				m1_released_ = true;
			break;
			case SDL_BUTTON_RIGHT:
				// right mouse button
			break;
			}
		break;
		case SDL_MOUSEMOTION:
			mouse_x_ = event.motion.x;
			mouse_y_ = event.motion.y;
		break;
		case SDL_QUIT:
			running_ = false;
		break;
	}
}

void App::RenderText(const char* text, TTF_Font* font, SDL_Color color, int x, int y) {
	SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
	SDL_FreeSurface(surface);
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect temp = { x, y, texW, texH };
	SDL_RenderCopy(renderer_, texture, NULL, &temp);
	SDL_DestroyTexture(texture);
}

SDL_Color App::color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	SDL_Color col = { r,g,b,a };
	return col;
}

void App::RenderBar(int x, int y, int w, int h, double max_value, double value, SDL_Color FGColor, SDL_Color BGColor) {
	double percentage = value > 0.f ? (value / max_value) : 0.f;
	SDL_Color old;
	SDL_GetRenderDrawColor(renderer_, &old.r, &old.g, &old.b, &old.a);
	SDL_Rect bgrect = { x, y, w, h };
	SDL_SetRenderDrawColor(renderer_, BGColor.r, BGColor.g, BGColor.b, BGColor.a);
	SDL_RenderFillRect(renderer_, &bgrect);
	SDL_SetRenderDrawColor(renderer_, FGColor.r, FGColor.g, FGColor.b, FGColor.a);
	SDL_Rect fgrect = {x, y, w - (int)(w * (1.f - percentage)), h};
	SDL_RenderFillRect(renderer_, &fgrect);
	std::stringstream ss;
	ss << (int)value << " / " << (int)max_value;
	RenderText(ss.str().c_str(), default_font_, { 255, 255, 255, 0 }, x + 5, y + 5);
	SDL_SetRenderDrawColor(renderer_, old.r, old.g, old.b, old.a);
}

void App::Render() {
	SDL_RenderClear(renderer_);

	// Render "renderables" aka. entities
	for (auto& i : to_render_) {
		int p = i -> GetVParent();
		if (p == -1 || (visible_[p] && !hidden_[p])) {
			i -> Render(renderer_, camera_x_, camera_y_);
		}
	}

	for (auto& i : entities_) {
		i -> Render(renderer_, camera_x_, camera_y_);
	}
	player_->Render(renderer_, camera_x_, camera_y_);

	// Render HUD
	for (auto i : hud_) {
		i->Render(renderer_, 0, 0);
	}

	SDL_Color red = color(255, 0, 0, 0);
	SDL_Color blue = color(0, 0, 192, 0);
	SDL_Color purple = color(192, 0, 192, 0);
	SDL_Color green = color(0, 192, 0, 0);
	SDL_Color grey = color(192, 192, 192, 0);
	double hp_percentage = player_->GetHealth() > 0 ? player_->GetHealth() / player_->GetMaxHealth() : 0.0;
	double mana_percentage = player_->GetMana() > 0 ? player_->GetMana() / player_->GetMaxMana() : 0.0;
	RenderBar(50, 10, 120, 30, player_->GetMaxHealth(), player_->GetHealth(), red, grey); // center x 340
	RenderBar(50, 60, 120, 30, player_->GetMaxMana(), player_->GetMana(), blue, grey);
	RenderBar(50, 110, 120, 30, player_->GetNextXp(), player_->GetXp(), green, grey);


	std::stringstream ss_hh;
	std::stringstream ss_mm;

	ss_hh << player_->GetHealthPotions();
	ss_mm << player_->GetManaPotions();

	RenderText("HP", default_font_, { 255, 255, 255, 0 }, 5, 15);
	RenderText("Mana", default_font_, { 255, 255, 255, 0 }, 5, 65);
	RenderText("XP", default_font_, { 255, 255, 255, 0 }, 5, 115);
	RenderText(ss_hh.str().c_str(), default_font_, { 255, 255, 255, 0 }, 380, 580);
	RenderText(ss_mm.str().c_str(), default_font_, { 255, 255, 255, 0 }, 438, 580);

	std::stringstream ss_lvl;
	std::stringstream ss_money;
	std::stringstream ss_o;

	ss_lvl << "Level: " << player_ -> GetLevel();
	ss_money << "Money: " << player_ -> GetMoney() << " gp";
	ss_o << "Stage: " << stage_;

	RenderText(ss_lvl.str().c_str(), default_font_, { 255, 255, 255, 0 }, 5, 150);
	RenderText(ss_money.str().c_str(), default_font_, { 255, 255, 255, 0 }, 5, 175);
	RenderText(ss_o.str().c_str(), default_font_, { 255, 255, 255, 0 }, 5, 200);

	std::stringstream ss_item;
	std::stringstream ss_item2;

	ItemType wep = player_->GetWeapon();
	double dmg = (wep == ItemType::staff) * 5.0 + (wep == ItemType::bow) * 4.0 + (wep == ItemType::sword) * 6.0;
	double spd = (wep == ItemType::staff) * 0.2 + (wep == ItemType::bow) * 0.4 + (wep == ItemType::sword) * 0.1;

	ss_item << "Damage: " << (dmg * damage_m_);
	ss_item2 << "Speed: " << ((1.0 - spd) * speed_m_);

	RenderText(ss_item.str().c_str(), default_font_, { 255, 255, 255, 0 }, 600, 5);
	RenderText(ss_item2.str().c_str(), default_font_, { 255, 255, 255, 0 }, 600, 30);

	if (bossptr != nullptr) {
		std::stringstream ss_boss;

		RenderBar(340, 5, 120, 30, bossptr->GetBoss()->GetMaxHealth(), bossptr->GetBoss()->GetHealth(), purple, grey);
		bossptr->BossPhase();
		if (bossptr->GetBoss()->GetHealth() <= 0) {
			for (auto& monster : monsters_)
			{
				monster->Kill();
				monster->SetTexture(textures_->Get(TextureType::tombstone));
				monster->AddVel(0, 0);
				monster->CalcPos(fps_desired_);
			}
			bossptr = nullptr;
		}
	}

	SDL_RenderPresent(renderer_);
}

bool App::Running() const {
	return running_;
}

void App::PlayerCastsProjectile(int previous_x, int previous_y) {
	double newMana;
	switch (player_->GetWeapon()) {
	case ItemType::staff:
		if (shoot_timer_ <= 0.0) {
			newMana = player_->GetMana() - 5.0;
			if (newMana >= 0.0) {
				player_->SetMana(newMana);
				AddProjectile(TextureType::fireball, previous_x + 8, previous_y + 8, 400, mouse_player_angle_, player_, ProjectileType::Fireball);
				PlaySound(0, 0);
			}
			shoot_timer_ = 0.2 * speed_m_;
		}
		// else staff could be melee maybe?
	break;

	case ItemType::sword:
		if (shoot_timer_ <= 0.0) {
			double xx, yy;
			xx = previous_x - 8.0 + 24.0 * cos(mouse_player_angle_ * M_PI / 180.0);
			yy = previous_y - 8.0 - 24.0 * sin(mouse_player_angle_ * M_PI / 180.0);
			AddProjectile(TextureType::melee, xx, yy, 1, mouse_player_angle_, player_, ProjectileType::Melee);
			shoot_timer_ = 0.1 * speed_m_;
			PlaySound(6, 0);
		}
	break;

	case ItemType::bow:
		if (shoot_timer_ <= 0.0) {
			AddProjectile(TextureType::arrow, previous_x + 8, previous_y + 8, 400, mouse_player_angle_, player_, ProjectileType::Arrow);
			shoot_timer_ = 0.4 * speed_m_;
			PlaySound(5, 0);
		}
	break;

	// case ItemType::sword, melee stuff here
	// cast ItemType::bow, bow stuff here
	// For balancing reasons bow should maybe have a shooting cooldown or just lower damage
	default: // For demonstration reasons, default should probably be melee
		AddProjectile(TextureType::arrow, previous_x + 8, previous_y + 8, 400, mouse_player_angle_, player_, ProjectileType::Arrow);
		PlaySound(0, 0); // This should be something else
	break;
	}
}

std::shared_ptr<Wall> App::AddWall(TextureType type, const int& x, const int& y) {
	std::shared_ptr<Wall> temp(new Wall(textures_->Get(type), x, y));
	temp-> SetVParent(int(y / 512) * dungeon_width_ + int(x / 512));
	to_render_.push_back(temp);
	walls_.push_back(temp);
	if (type != invisible) {
		path_tiles_[unsigned((floor(y / 32)) * room_width_ / 32 + (floor(x / 32)))] = false;
	}
	return temp;
}

void App::AddFloor(TextureType type, const int& x, const int& y) {
	std::shared_ptr<Floor> temp(new Floor(textures_->Get(type), x, y));
	temp -> SetVParent(int(y / 512) * dungeon_width_ + int(x / 512));
	to_render_.push_back(temp);
	floor_.push_back(temp);
}

std::shared_ptr<Door> App::AddDoor(TextureType type, const int& x, const int& y) {
	std::shared_ptr<Door> temp(new Door(textures_->Get(type), x, y));
	temp -> SetVParent(int(y / 512) * dungeon_width_ + int(x / 512));
	to_render_.push_back(temp);
	doors_.push_back(temp);
	return temp;
}

void App::AddProjectile(TextureType type, const int& x, const int& y,double speed, double dir, std::shared_ptr<Renderable> parent,ProjectileType pro) {
	std::shared_ptr<Projectile> temp = nullptr;
	double angle = 2 * M_PI - dir;
	switch (pro) {
	case ProjectileType::Fireball: 
		temp = std::shared_ptr<Projectile>(new Fireball(textures_->Get(type), x, y, speed, dir, parent));
		temp -> SetAngle(angle);
		temp -> SetImageSpeed(4);
		break;
	case ProjectileType::IceBall:
		temp = std::shared_ptr<Projectile>(new IceBall(textures_->Get(type), x, y, speed, dir, parent));
		temp -> SetAngle(angle);
		temp -> SetImageSpeed(4);
		break;
	case ProjectileType::Melee: 
		temp = std::shared_ptr<Projectile>(new Melee(textures_->Get(type), x, y, 0.0, dir, parent));
		temp -> SetAngle(angle);
		temp -> SetImageSpeed(4);
		break;
	case ProjectileType::Arrow:
		temp = std::shared_ptr<Projectile>(new Arrow(textures_->Get(type), x, y, speed, dir, parent));
		temp -> SetAngle(angle);
		temp-> SetImageSpeed(4);
		break;
	}
	entities_.push_back(temp);
	projectiles_.push_back(temp);
}

void App::AddChest(TextureType type, const int& x, const int& y) {
	std::shared_ptr<Chest> temp(new Chest(textures_->Get(type), x, y));
	temp -> SetVParent(int(y / 512) * dungeon_width_ + int(x / 512));
	to_render_.push_back(temp);
	chests_.push_back(temp);
}

void App::LoadSound(const char* path) {
	sounds_.push_back(Mix_LoadWAV(path));
}

void App::PlaySound(const unsigned& index, const int& loops) {
	Mix_PlayChannel(-1, sounds_[index], loops);
}

void App::LoadRoom(const char* path) {
	Room temp_room;
	ifstream file(path);
	string line;
	vector<int> temp;

	string name = path;
	//North East South West
	bool dir[4] = { 0, 0, 0, 0 };
	for (unsigned i = 0; i < 4; i++) {
		temp_room.dir[i] = ((path[name.length() - 5 - i]) == '1');
		dir[i] = ((path[name.length() - 5 - i]) == '1');
	}
	for (unsigned i = 0; i < 4; i++) {
		temp_room.dir[i] = dir[3 - i];
	}

	unsigned w = 0, h = 0;

	while (getline(file, line)) {
		stringstream ss(line);
		w = 0;
		for (unsigned i = 0; i < line.length(); i++) {
			char a;
			ss >> a;
			temp.push_back((int)a);
			w++;
		}
		h++;
	}

	temp_room.width = w;
	temp_room.height = h;
	temp_room.data = temp;

	room_data_.push_back(temp_room);
}

bool App::AddRoom(const unsigned int& index, const int& x, const int& y) {
	unsigned w, h;
	w = room_data_[index].width;
	h = room_data_[index].height;

	for (unsigned j = 0; j < h; j++) {
		for (unsigned i = 0; i < w; i++) {
			if (unsigned((floor(y / 32) + j) * room_width_ / 32 + (floor(x / 32) + i)) > size_ - 1) {
				return false;
			}
			if (tiles_[unsigned((floor(y / 32) + j) * room_width_ / 32 + (floor(x / 32) + i))]) {
				return false;
			}
		}
	}

	for (unsigned j = 0; j < h; j++) {
		for (unsigned i = 0; i < w; i++) {
			if ((x + i * 32 < room_width_) && (y + i * 32 < room_height_)) {
				unsigned t = (j * 32 + y) / 512 * dungeon_width_ + (i * 32 + x) / 512;
				switch (room_data_[index].data[unsigned (j * h + i)]) {
				default:
				break;
				case 45:
					// Air
					AddFloor(TextureType::dfloor, i * 32 + x, j * 32 + y);
				break;
				case 66:
					AddFloor(TextureType::dfloor, i * 32 + x, j * 32 + y);
					bossptr = std::shared_ptr<BossHandler>(
						new BossHandler(
							monster::AddMonster(monsters_, textures_, i * 32 + x, j * 32 + y, difficulty_,
								(rand() % 2 == 0) ? MonsterType::necromancer : MonsterType::banshee),
							monsters_,
							entities_,
							textures_,
							path_tiles_,
							tiles_,
							size_,
							room_width_,
							difficulty_
						));

					entities_.emplace_front(monsters_[monsters_.size() - 1]);
				break;
				case 68:
					// Mark this location for door generation
					door_spots_.push_back(unsigned((floor(y / 32) + j) * room_width_ / 32 + (floor(x / 32) + i)));
					door_spots_.push_back(0);
				break;
				case 69:
					AddFloor(TextureType::ladderdown, i * 32 + x, j * 32 + y);
					ladders_.push_back(shared_ptr<Ladder>(new Ladder(textures_->Get(invisible), i * 32 + x, j * 32 + y)));
				break;
				case 77:
					AddFloor(TextureType::dfloor, i * 32 + x, j * 32 + y);
					room_[t].monster_spawns.push_back((int)((floor(y / 32) + j) * room_width_ / 32 + (floor(x / 32) + i)));
				break;
				case 80:
					AddWall(TextureType::fire, i * 32 + x, j * 32 + y);
				break;
				case 86:
					// Void, add nothing
				break;
				case 87:
					AddWall(TextureType::wall2, i * 32 + x, j * 32 + y);
				break;

				case 88:
					AddFloor(TextureType::dfloor, i * 32 + x, j * 32 + y);
					AddFloor(TextureType::ez, i * 32 + x, j * 32 + y);
				break;
				case 89:
					AddFloor(TextureType::dfloor, i * 32 + x, j * 32 + y);
					AddFloor(TextureType::med, i * 32 + x, j * 32 + y);
				break;
				case 90:
					AddFloor(TextureType::dfloor, i * 32 + x, j * 32 + y);
					AddFloor(TextureType::hard, i * 32 + x, j * 32 + y);
				break;


				case 72:
					AddFloor(TextureType::dfloor, i * 32 + x, j * 32 + y);
					AddChest(TextureType::chest, i * 32 + x, j * 32 + y);
				break;
				case 100:
					// Mark this location for door generation
					door_spots_.push_back(unsigned((floor(y / 32) + j) * room_width_ / 32 + (floor(x / 32) + i)));
					door_spots_.push_back(1);
				break;
				case 112:
					player_ -> SetPos(i * 32 + x, j * 32 + y);
					AddFloor(TextureType::ladderup, i * 32 + x, j * 32 + y);
				break;
				}

				// Mark the tiles as used, can't generate on top.
				tiles_[unsigned((floor(y / 32) + j) * room_width_ / 32 + (floor(x / 32) + i))] = true;
			}
		}
	}

	return true;
}

void App::Generate() {
	// TODO: This requires a lot of cleanup! But it works for now

	auto rooms = room_data_.size();

	unsigned int max_x = 32, max_y = 32;
	auto room_size = max_x * max_y;
	int* room = new int[room_size];
	for (unsigned j = 0; j < max_x; j++) {
		for (unsigned i = 0; i < max_y; i++) {
			room[j * max_x + i] = -1;
		}
	}
	int x = 15, y = 15;
	room[y * max_x + x] = 0;
	int dir = 1 + rand() % 2;
	x = x + (dir == 1) - (dir == 3);
	y = y + -(dir == 0) + (dir == 2);
	unsigned int max_rooms = 15;
	unsigned int devrooms = 3;
	int index = 0;
	int pindex = 0;

	vector<int> tlist;

	// Generate initial path to the end room

	for (unsigned i = 0; i < max_rooms; i++) {
		tlist.clear();
		for (unsigned j = devrooms; j < rooms; j++) {
			if (room_data_[pindex].dir[dir] == room_data_[j].dir[(dir + 2) % 4]) {
				tlist.push_back(j);
			}
		}
		if (tlist.size() > 0) {
			index = tlist[rand() % (tlist.size())];
			while (index == pindex && tlist.size() > 1) {
				index = tlist[rand() % (tlist.size())];
			}
		} else {
			index = devrooms;
		}

		if (i == max_rooms - 1) {
			index = 1;
		}

		bool cdir[4];
		copy(begin(room_data_[index].dir), end(room_data_[index].dir), begin(cdir));

		unsigned int nx, ny, n = 0;
		
		for (unsigned int j = 0; j < 4; j++) {
			nx = x + (j == 1) - (j == 3);
			ny = y - (j == 0) + (j == 2);
			if (nx < 0 || nx >= max_x || ny < 0 || ny >= max_y) {
				cdir[j] = 0;
			} else {
				cdir[j] = cdir[j] * (room[ny * max_x + nx] == -1);
			}
			n += (cdir[j] == 1);
		}
		
		if (n > 0) {
			dir = rand() % 4;
			while (cdir[dir] == 0) {
				dir = (dir + 1) % 4;
			}
		} else {
			index = 3;
		}

		bool exit = false;
		if (room[y * max_x + x] != -1) {
			index = 1;
			exit = true;
		}

		room[y * max_x + x] = index;
		pindex = index;

		x = x + (dir == 1) - (dir == 3);
		y = y - (dir == 0) + (dir == 2);
		if (exit) {
			break;
		}
	}

	// Adding dead-end rooms, fill with items, monsters, etc later.

	for (unsigned j = 0; j < max_x; j++) {
		for (unsigned i = 0; i < max_y; i++) {
			if (room[j * max_x + i] != -1 && room[j * max_x + i] != 1 && room[j * max_x + i] != 2) {
				bool cdir[4];
				copy(begin(room_data_[room[j * max_x + i]].dir), end(room_data_[room[j * max_x + i]].dir), begin(cdir));
				unsigned nx, ny, n = 0;
				for (unsigned k = 0; k < 4; k++) {
					nx = i + (k == 1) - (k == 3);
					ny = j - (k == 0) + (k == 2);
					if (nx < 0 || nx >= max_x || ny < 0 || ny >= max_y) {
						cdir[k] = 0;
					} else {
						cdir[k] = cdir[k] * (room[ny * max_x + nx] == -1);
					}
				}
				for (unsigned k = 0; k < 4; k++) {
					if (cdir[k] != 0) {
						nx = i + (k == 1) - (k == 3);
						ny = j - (k == 0) + (k == 2);
						room[ny * max_x + nx] = 2;
					}
				}
			}
		}
	}

	// Shift the map

	int min_i = 32, min_j = 32, max_i = 0, max_j = 0;

	for (unsigned j = 0; j < max_x; j++) {
		for (unsigned i = 0; i < max_y; i++) {
			if (room[j * max_x + i] != -1) {
				min_i = min((int)i, min_i);
				min_j = min((int)j, min_j);
				max_i = max((int)i, max_i);
				max_j = max((int)j, max_j);
			}
		}
	}

	for (unsigned j = 0; j < max_x; j++) {
		for (unsigned i = 0; i < max_y; i++) {
			if (room[j * max_x + i] > -1) {
				auto w = room_data_[room[j * max_x + i]].width, h = room_data_[room[j * max_x + i]].height;
				room_width_ = max(room_width_, (i + 1 - min_i) * 32 * w);
				room_height_ = max(room_height_, (j + 1 - min_j) * 32 * h);
			}
		}
	}

	dungeon_width_ = (max_i - min_i) + 1;
	dungeon_height_ = (max_j - min_j) + 1;

	hidden_ = new bool[dungeon_height_ * dungeon_width_];
	visible_ = new bool[dungeon_height_ * dungeon_width_];
	room_ = new DungeonRoom[dungeon_height_ * dungeon_width_];

	cout << "Printing map: " << endl;
	for (unsigned j = 0; j < dungeon_height_; j++) {
		for (unsigned i = 0; i < dungeon_width_; i++) {
			visible_[unsigned(j * dungeon_width_ + i)] = false;
			hidden_[unsigned(j * dungeon_width_ + i)] = true && (!fullvis_);
			int r = room[(j + min_j) * max_x + (i + min_i)];
			DungeonRoom temp;
			temp.index = r;
			room_[unsigned(j * dungeon_width_ + i)] = temp;
			if (r != -1) {
				if (r == 0) {
					hidden_[unsigned(j * dungeon_width_ + i)] = false;
				}
				cout << r << " ";
			} else {
				cout << "# ";
			}
		}
		cout << "\n";
	}
	cout << "Room size: " << room_width_ << " x " << room_height_ << endl;

	size_ = (room_width_ / 32 * room_height_ / 32);
	tiles_ = new bool[size_];
	path_tiles_ = new bool[size_];
	for (unsigned i = 0; i < size_; i++) {
		tiles_[i] = false;
		path_tiles_[i] = true;
	}

	// Add rooms and close them down if they're not connected to anything

	
	for (unsigned j = 0; j < max_x; j++) {
		for (unsigned i = 0; i < max_y; i++) {
			if (room[j * max_x + i] > -1) {
				auto w = room_data_[room[j * max_x + i]].width, h = room_data_[room[j * max_x + i]].height;
				AddRoom(room[j * max_x + i], (i - min_i) * 32 * w, (j - min_j) * 32 * h);

				bool cdir[4];
				copy(begin(room_data_[room[j * max_x + i]].dir), end(room_data_[room[j * max_x + i]].dir), begin(cdir));
				
				unsigned nx, ny;
				for (unsigned k = 0; k < 4; k++) {
					nx = i + (k == 1) - (k == 3);
					ny = j - (k == 0) + (k == 2);
					if (!(nx < 0 || nx >= max_x || ny < 0 || ny >= max_y)) {
						cdir[k] = cdir[k] * (room[ny * max_x + nx] == -1);
					}
					if (cdir[k] == 0) {
						if (room[ny * max_x + nx] != -1) {
							if (room_data_[room[ny * max_x + nx]].dir[(k + 2) % 4] != room_data_[room[j * max_x + i]].dir[k] && room_data_[room[j * max_x + i]].dir[k] == 1) {
								cdir[k] = 1;
							}
						}
					}
				}
				bool ddir[4];
				copy(begin(room_data_[room[j * max_x + i]].dir), end(room_data_[room[j * max_x + i]].dir), begin(ddir));

				for (unsigned k = 0; k < 4; k++) {
					nx = i + (k == 1) - (k == 3);
					ny = j - (k == 0) + (k == 2);
					if (!(nx < 0 || nx >= max_x || ny < 0 || ny >= max_y)) {
						ddir[k] = ddir[k] * (room[ny * max_x + nx] != -1);
					}

				}

				DungeonRoom& temp = room_[unsigned((j - min_j) * dungeon_width_ + (i - min_i))];
				copy(begin(ddir), end(ddir), begin(temp.dir));

				for (unsigned k = 0; k < 4; k++) {
					if (cdir[k] == 1) {
						nx = (i - min_i) * 32 * w;
						ny = (j - min_j) * 32 * h;
						switch (k) {
							default:
							break;
							case 0:
								nx += (w / 2 * 32);
								AddWall(TextureType::wall2, nx - 32, ny);
								AddWall(TextureType::wall2, nx, ny);
							break;
							case 1:
								nx += (w - 1) * 32;
								ny += (h / 2 * 32);
								AddWall(TextureType::wall2, nx, ny - 32);
								AddWall(TextureType::wall2, nx, ny);
							break;
							case 2:
								nx += (w / 2 * 32);
								ny += (h - 1) * 32;
								AddWall(TextureType::wall2, nx - 32, ny);
								AddWall(TextureType::wall2, nx, ny);
							break;
							case 3:
								ny += (h / 2 * 32);
								AddWall(TextureType::wall2, nx, ny - 32);
								AddWall(TextureType::wall2, nx, ny);
							break;
						}
					}
				}
			}
		}
	}

	for (unsigned i = 0; i < door_spots_.size() / 2; i++) {
		int j = door_spots_[i * 2];
		if (path_tiles_[j]) {
			int nx = (j % (room_width_ / 32)) * 32;
			int ny = (int)floor(j / (room_width_ / 32)) * 32;

			int cx = (int)floor(nx / 512) * 512 + 256;
			int cy = (int)floor(ny / 512) * 512 + 256;
			
			AddFloor(TextureType::dfloor, nx, ny);

			int t = (int)floor(ny / 512) * dungeon_width_ + (int)floor(nx / 512);
			DungeonRoom& temp = room_[t];

			if (door_spots_[i * 2 + 1]) {
				if (nx > cx) {
					temp.door[1] = 1;
				} else {
					temp.door[3] = 1;
				}
			} else {
 				if (ny > cy) {
					temp.door[2] = 1;
				} else {
					temp.door[0] = 1;
				}
			}
		}
	}

	for (unsigned i = 0; i < door_spots_.size() / 2; i++) {
		int j = door_spots_[i * 2];
		if (path_tiles_[j]) {
			int nx = (j % (room_width_ / 32)) * 32;
			int ny = (int)floor(j / (room_width_ / 32)) * 32;

			int cx = (int)floor(nx / 512) * 512 + 256;
			int cy = (int)floor(ny / 512) * 512 + 256;
			
			AddFloor(TextureType::dfloor, nx, ny);

			int t = (int)floor(ny / 512) * dungeon_width_ + (int)floor(nx / 512);

			if (door_spots_[i * 2 + 1]) {
				if (nx > cx) {
					std::shared_ptr<Door> a = AddDoor(TextureType::vdoor1, nx, ny);
					Recursive(t + 1, a, 3);
					a -> SetFirst(t);
					a -> SetTiles(j, j + (room_width_ / 32));

					std::shared_ptr<Wall> wall0 = AddWall(TextureType::invisible, nx + 16, ny);
					std::shared_ptr<Wall> wall1 = AddWall(TextureType::invisible, nx + 16, ny + 32);

					a -> SetWalls(wall0, wall1);

				} else {
					std::shared_ptr<Door> a = AddDoor(TextureType::vdoor2, nx, ny);
					Recursive(t - 1, a, 1);
					a -> SetFirst(t);
					a -> SetTiles(j, j + (room_width_ / 32));

					std::shared_ptr<Wall> wall0 = AddWall(TextureType::invisible, nx - 16, ny);
					std::shared_ptr<Wall> wall1 = AddWall(TextureType::invisible, nx - 16, ny + 32);

					a->SetWalls(wall0, wall1);
				}
				path_tiles_[j] = false;
				path_tiles_[j + (room_width_ / 32)] = false;
			} else {
 				if (ny > cy) {
					std::shared_ptr<Door> a = AddDoor(TextureType::hdoor1, nx, ny);
					Recursive(t + dungeon_width_, a, 0);
					a -> SetFirst(t);
					a -> SetTiles(j, j + 1);

					std::shared_ptr<Wall> wall0 = AddWall(TextureType::invisible, nx, ny + 16);
					std::shared_ptr<Wall> wall1 = AddWall(TextureType::invisible, nx + 32, ny + 16);

					a->SetWalls(wall0, wall1);
				} else {
					std::shared_ptr<Door> a = AddDoor(TextureType::hdoor2, nx, ny);
					Recursive(t - dungeon_width_, a, 2);
					a -> SetFirst(t);
					a -> SetTiles(j, j + 1);
					std::shared_ptr<Wall> wall0 = AddWall(TextureType::invisible, nx, ny - 16);
					std::shared_ptr<Wall> wall1 = AddWall(TextureType::invisible, nx + 32, ny - 16);

					a->SetWalls(wall0, wall1);
				}
				path_tiles_[j] = false;
				path_tiles_[j + 1] = false;
			}
		}
	}

	bool has_start = false;
	bool has_end = false;

	for (unsigned i = 0; i < dungeon_width_ * dungeon_height_; i++) {
		if (room_[i].index == 0) {
			has_start = true;
		} else if (room_[i].index == 1) {
			has_end = true;
		}
	}

	if (!(has_start && has_end)) {
		Reset();
		Generate();
	}

}

void App::Recursive(const unsigned int& index, std::shared_ptr<Door> pointer, const unsigned int& previous_dir) {
	pointer -> AddConnected(index);
	bool cdir[4];
	copy(begin(room_[index].dir), end(room_[index].dir), begin(cdir));
	bool ddir[4];
	copy(begin(room_[index].door), end(room_[index].door), begin(ddir));

	if (!ddir[previous_dir]) {
		for (unsigned k = 0; k < 4; k++) {
			if (k != previous_dir) {
				if (cdir[k] && !ddir[k]) {
					int ni = index + (k == 1) - (k == 3) + (-(k == 0) + (k == 2)) * dungeon_width_;
					Recursive(ni, pointer, (k + 2) % 4);
				}
			}
		}
	} else {
		last_dir_ = previous_dir;
	}
}

void App::Reset() {
	tiles_ = nullptr;
	path_tiles_ = nullptr;
	hidden_ = nullptr;
	visible_ = nullptr;
	room_ = nullptr;
	bossptr = nullptr;
	dungeon_width_ = 0;
	dungeon_height_ = 0;
	door_spots_.clear();

	floor_.clear();
	walls_.clear();
	doors_.clear();
	monsters_.clear();
	projectiles_.clear();
	chests_.clear();
	items_.clear();
	entities_.clear();
	to_render_.clear();
	ladders_.clear();
	room_width_ = 1024;
	room_height_ = 1024;
}

void App::MainMenu() {
	room_data_.clear();
	room_width_ = 1440;
	room_height_ = 1440;
	LoadRoom("assets/mainmenu_0000.txt");

	size_ = (room_width_ / 32 * room_height_ / 32);
	tiles_ = new bool[size_];
	path_tiles_ = new bool[size_];
	for (unsigned i = 0; i < size_; i++) {
		tiles_[i] = false;
		path_tiles_[i] = true;
	}

	hidden_ = new bool[1];
	visible_ = new bool[1];
	room_ = new DungeonRoom[1];

	AddRoom(0, 480, 480);

	for (auto& i : to_render_) {
		i->SetVParent(-1);
	}
}

void App::BossBattle() {
	room_data_.clear();
	room_width_ = 2048;
	room_height_ = 2048;
	LoadRoom("assets/boss_0000.txt");

	size_ = (room_width_ / 32 * room_height_ / 32);
	tiles_ = new bool[size_];
	path_tiles_ = new bool[size_];
	for (unsigned i = 0; i < size_; i++) {
		tiles_[i] = false;
		path_tiles_[i] = true;
	}

	hidden_ = new bool[1];
	visible_ = new bool[1];
	room_ = new DungeonRoom[1];

	AddRoom(0, 512, 512);

	for (auto& i : to_render_) {
		i->SetVParent(-1);
	}
}