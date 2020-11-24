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
		window_ = SDL_CreateWindow(title_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_, (fullscreen_ ? (int) SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN));
		renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
		SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);

		LoadTexture("assets/player_8.png");
		LoadTexture("assets/wall2_1.png");
		LoadTexture("assets/fire_3.png");
		LoadTexture("assets/monster_1.png");
		LoadTexture("assets/potion1_1.png");
		LoadTexture("assets/potion2_1.png");
		LoadTexture("assets/chest_2.png");

		player_ = new Player(textures_[0], 0, 0);

		LoadRoom("assets/startroom_0110.txt");
		LoadRoom("assets/endroom_1111.txt");
		LoadRoom("assets/deadend_1111.txt");

		LoadRoom("assets/largeroom_1111.txt");

		LoadRoom("assets/largecorner_0011.txt");
		LoadRoom("assets/largecorner_1001.txt");
		LoadRoom("assets/largetunnel_0101.txt");
		LoadRoom("assets/largetunnel_1010.txt");
		Generate();

		for (auto& monster : monsters_) {
			to_render_.push_back(monster);
		}

		to_render_.push_back(player_);

		running_ = true;
	} else {
		running_ = false;
	}
}

App::~App() {
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}

void App::Update() {
	now_ = (double)SDL_GetTicks();
	delta_time_ = max((now_ - last_) / 1000.0f * fps_desired_, 1.0 / 1000.0f);
	second_timer_ += delta_time_ / fps_desired_;
	tick_timer_ += delta_time_;
	double previous_x, previous_y;
	player_ -> GetPos(previous_x, previous_y);

	// Update 4 times per second

	if (second_timer_ >= 0.25) {
		second_timer_ -= 0.25;
		update::CalculatePath(monsters_, path_tiles_, previous_x, previous_y, size_, room_width_, room_height_);
	}
	
	if (f_) {//check if player and item or chest intersect
		f_ = false;
		SDL_Rect rect1 = player_->ReturnRect();
		if (!items_.empty()){
			for (auto& i : items_) {
				SDL_Rect rect2 = i->ReturnRect();
				if (SDL_HasIntersection(&rect1, &rect2)) {
					i -> Pickup();
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
					AddItem(x1, y1);
					break;
				}
			}
		}
	}
	if (m1_) {
		m1_ = false;
		AddProjectile(2, previous_x, previous_y, 512.0, mouse_player_angle_);
	}

	double x1, y1;
	int w1, h1;
	player_ -> GetPos(x1, y1);
	player_ -> GetRect(w1, h1);

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
				if (fabs(y_res) > 0.0) {
					player_ -> AddPos(0, -y_res);
					player_ -> SetVel(1, 0.0);
				}
			} else if (fabs(y_res) >= 4.0) {
				if (fabs(x_res) > 0.0) {
					player_ -> AddPos(-x_res, 0);
					player_ -> SetVel(0, 0.0);
				}
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
			i -> AddFrame(delta_time_);
		}

		if (!monsters_.empty()) {
			update::UpdateMonsters(monsters_, delta_time_, fps_desired_,
				up_ || down_ || left_ || right_,
				room_width_, walls_);
		}

		for (auto& i : projectiles_) {
			if (i->GetActive()) {
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
				for (auto& j : monsters_) {
					double x3, y3;
					int w3, h3;
					j->GetPos(x3, y3);
					j->GetRect(w3, h3);
					if (x2 < ((double)x3 + w3) && x2 + w2 > x3 && y2 < ((double)y3 + h3) && y2 + h2 > y3) {
						//i->SetVel(1, 0);
						//i->SetVel(0, 0);
						i -> SetActive(false);
					}
				}
			}
		}
		while (tick_timer_ >= 1.0) {
			tick_timer_ -= 1.0;
		}
	}
	//despawn items
	if (!items_.empty()){
		for (auto i : items_) {
			if (!i->Spawned()) {
				to_render_.erase(remove(to_render_.begin(), to_render_.end(), i), to_render_.end());
				items_.erase(remove(items_.begin(), items_.end(), i), items_.end());
				delete i;
			}
		}
	}

	if (projectiles_.size() > 0) {
		for (auto i : projectiles_) {
			if (!i -> GetActive()) {
				to_render_.erase(remove(to_render_.begin(), to_render_.end(), i), to_render_.end());
				projectiles_.erase(remove(projectiles_.begin(), projectiles_.end(), i), projectiles_.end());
				delete i;
			}
		}
	}

	player_ -> GetPos(camera_x_, camera_y_);

	double x_offset = width_ / 2.0;
	double y_offset = height_ / 2.0;

	camera_x_ = floor(camera_x_ - x_offset + 0.5 + w1 / 2);
	camera_y_ = floor(camera_y_ - y_offset + 0.5 + h1 / 2);

	camera_x_ = min(max(0.0, camera_x_), max(0.0, room_width_ - x_offset * 2));
	camera_y_ = min(max(0.0, camera_y_), max(0.0, room_height_ - y_offset * 2));

	mouse_player_angle_ = fmod(540.0 - atan2f((y1 + 16 - camera_y_ - mouse_y_), (x1 + 16 - camera_x_ - mouse_x_)) * 180.0 / M_PI, 360.0);
	
	
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

void App::Render() {
	SDL_RenderClear(renderer_);

	// Render "renderables" aka. entities
	for (auto& i : to_render_) {
		i -> Render(renderer_, camera_x_, camera_y_);
	}
	// Render HUD

	SDL_RenderPresent(renderer_);
}

bool App::Running() const {
	return running_;
}

void App::AddWall(const size_t& index, const int& x, const int& y) {
	Wall* temp = new Wall(textures_[index], x, y);
	to_render_.push_back(temp);
	walls_.push_back(temp);
	path_tiles_[unsigned((floor(y / 32)) * room_width_ / 32 + (floor(x / 32)))] = false;
}
void App::AddProjectile(const size_t& index, const int& x, const int& y,double speed, double dir) {
	Projectile* temp = new Projectile(textures_[index], x, y,speed,dir);
	to_render_.push_back(temp);
	projectiles_.push_back(temp);
}
void App::AddItem(const int& x, const int& y, ItemType type) {
	if (type == ItemType::random) { //random if ItemType not given
		type = static_cast<ItemType>(rand() % (int)(ItemType::random));
	}
	size_t index = 0;
	switch (type) { //get index for texture
	case ItemType::health_potion:
		index = 4;
		break;
	case ItemType::mana_potion:
		index = 5;
	break;
	}
	Item* temp = new Item(textures_[index], x, y, type);
	to_render_.push_back(temp);
	items_.push_back(temp);
}
void App::AddChest(const size_t& index, const int& x, const int& y) {
	Chest* temp = new Chest(textures_[index], x, y);
	to_render_.push_back(temp);
	chests_.push_back(temp);
}

void App::LoadTexture(const char* path) {
	Texture temp_texture;
	
	SDL_Surface* temp = IMG_Load(path);
	
	SDL_Rect temp_rect;
	temp_rect.w = temp -> w;
	temp_rect.h = temp -> h;
	temp_rect.x = 0;
	temp_rect.y = 0;

	temp_texture.texture = SDL_CreateTextureFromSurface(renderer_, temp);
	
	string name = path;
	string images;
	char t;
	unsigned i = 0;

	while (true) {
		t = path[name.length() - 5 - i];
		if (t != '_') {
			images += t;
			i++;
		} else {
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
				switch (room_data_[index].data[unsigned (j * h + i)]) {
				default:
				break;
				case 45:
					// Air
					//path_tiles_[unsigned((floor(y / 32) + j) * room_width_ / 32 + (floor(x / 32) + i))] = true;
				break;
				case 77:
					//path_tiles_[unsigned((floor(y / 32) + j) * room_width_ / 32 + (floor(x / 32) + i))] = true;
					monsters_.push_back(new Monster(textures_[3], i * 32 + x, j * 32 + y));
				break;
				case 80:
					AddWall(2, i * 32 + x, j * 32 + y);
					//path_tiles_[unsigned((floor(y / 32) + j) * room_width_ / 32 + (floor(x / 32) + i))] = false;
				break;
				case 87:
					AddWall(1, i * 32 + x, j * 32 + y);
					//path_tiles_[unsigned((floor(y / 32) + j) * room_width_ / 32 + (floor(x / 32) + i))] = false;
				break;
				case 72:
					AddChest(6, i * 32 + x, j * 32 + y);
				break;
				case 112:
					player_ -> SetPos(i * 32 + x, j * 32 + y);
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
	srand(time(NULL));

	auto rooms = room_data_.size();

	int max_x = 32, max_y = 32;
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
	int max_rooms = 15;
	int devrooms = 3;
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

		room[y * max_x + x] = index;
		pindex = index;

		bool cdir[4];
		copy(begin(room_data_[index].dir), end(room_data_[index].dir), begin(cdir));

		int nx, ny, n = 0;
		
		for (unsigned j = 0; j < 4; j++) {
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
		}

		x = x + (dir == 1) - (dir == 3);
		y = y - (dir == 0) + (dir == 2);
	}

	// Adding dead-end rooms, fill with items, monsters, etc later.

	for (unsigned j = 0; j < max_x; j++) {
		for (unsigned i = 0; i < max_y; i++) {
			if (room[j * max_x + i] != -1 && room[j * max_x + i] != 1 && room[j * max_x + i] != 2) {
				bool cdir[4];
				copy(begin(room_data_[room[j * max_x + i]].dir), end(room_data_[room[j * max_x + i]].dir), begin(cdir));
				int nx, ny, n = 0;
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

	size_ = (room_width_ / 32 * room_height_ / 32);
	tiles_ = new bool[size_];
	path_tiles_ = new bool[size_];
	for (unsigned i = 0; i < size_; i++) {
		tiles_[i] = false;
		path_tiles_[i] = true;
	}

	// Add rooms and close them down if they're not connected to anything

	cout << "Printing map: " << endl;
	for (unsigned j = 0; j < max_x; j++) {
		for (unsigned i = 0; i < max_y; i++) {
			if (room[j * max_x + i] > -1) {
				auto w = room_data_[room[j * max_x + i]].width, h = room_data_[room[j * max_x + i]].height;
				AddRoom(room[j * max_x + i], (i - min_i) * 32 * w, (j - min_j) * 32 * h);

				bool cdir[4];
				copy(begin(room_data_[room[j * max_x + i]].dir), end(room_data_[room[j * max_x + i]].dir), begin(cdir));
				int nx, ny;
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
				for (unsigned k = 0; k < 4; k++) {
					if (cdir[k] == 1) {
						nx = (i - min_i) * 32 * w;
						ny = (j - min_j) * 32 * h;
						switch (k) {
							default:
							break;
							case 0:
								nx += (w / 2 * 32);
								AddWall(1, nx - 32, ny);
								AddWall(1, nx, ny);
							break;
							case 1:
								nx += (w - 1) * 32;
								ny += (h / 2 * 32);
								AddWall(1, nx, ny - 32);
								AddWall(1, nx, ny);
							break;
							case 2:
								nx += (w / 2 * 32);
								ny += (h - 1) * 32;
								AddWall(1, nx - 32, ny);
								AddWall(1, nx, ny);
							break;
							case 3:
								ny += (h / 2 * 32);
								AddWall(1, nx, ny - 32);
								AddWall(1, nx, ny);
							break;
						}
						
					}
				}
				cout << room[j * max_x + i] << " ";
			} else {
				cout << "# ";
			}

		}
		cout << "\n";
	}
	cout << "Room size: " << room_width_ << " x " << room_height_ << endl;
}