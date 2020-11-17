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
		SDL_SetRenderDrawColor(renderer_, 32, 32, 32, 255);

		LoadTexture("assets/player_8.png");
		LoadTexture("assets/wall_1.png");
		LoadTexture("assets/fire_3.png");
		LoadTexture("assets/monster_1.png");

		player_ = new Player(textures_[0], 64, 64);

		LoadRoom("assets/room_1111.txt");
		LoadRoom("assets/room2_1111.txt");

		size_ = (room_width_ / 32 * room_height_ / 32);
		tiles_ = new bool[size_];
		path_tiles_ = new bool[size_];
		for (unsigned i = 0; i < size_; i++) {
			tiles_[i] = false;
			path_tiles_[i] = false;
		}

		//AddRoom(0, 32, 32);
		//AddRoom(0, 32, 288);
		//AddRoom(1, 288, 224);
		
		// TODO: recursive generation function

		srand(2);

		int x = 32;
		int y = 32;
		int prevh = -1;
		for (unsigned i = 0; i < 4; i++) {
			unsigned int index = rand() % 2;
			if (prevh == -1) {
				prevh = room_data_[index].height;
			}
			y += ((prevh - (int)room_data_[index].height) / 2) * 32;
			if (AddRoom(index, x, y)) {
				x += room_data_[index].width * 32;
				prevh = room_data_[index].height;
			}
			
		}

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
	last_ = now_;
	now_ = SDL_GetPerformanceCounter();
	delta_time_ = (double)((now_ - last_) * 1000.0 / (double)SDL_GetPerformanceFrequency());
	second_timer_ += delta_time_;
	double previous_x, previous_y;
	player_->GetPos(previous_x, previous_y);

	if (second_timer_ >= 1.0) {
		second_timer_ -= 1.0;
		update::CalculatePath(monsters_, path_tiles_, previous_x, previous_y,
			size_, room_width_, room_height_);
	}

	double speed = player_ -> GetSpeed() / fps_desired_ * delta_time_;


	if (!monsters_.empty()) {
		update::UpdateMonsters(monsters_, fps_desired_, 1.0/fps_desired_ * delta_time_,
			up_ || down_ || left_ || right_,
			room_width_, room_height_);
	}

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
	if (space_) {
		space_ = false;
		AddProjectile(2, previous_x, previous_y,5.0, mouse_player_angle_);
	}
	

	player_ -> CalcPos(fps_desired_);

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

			if (fabs(x_res) >= 1.0) {
				if (fabs(y_res) > 0.0) {
					player_ -> AddPos(0, -y_res);
					player_ -> SetVel(1, 0.0);
				}
			} else if (fabs(y_res) >= 1.0) {
				if (fabs(x_res) > 0.0) {
					player_ -> AddPos(-x_res, 0);
					player_ -> SetVel(0, 0.0);
				}
			}
		}
	}

	for (auto& i : projectiles_) {
		i->CalcPos(fps_desired_);
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
				i->SetVel(1, 0);
				i->SetVel(0, 0);
			}
		}
		for (auto& j : monsters_) {
			double x3, y3;
			int w3, h3;
			j->GetPos(x3, y3);
			j->GetRect(w3, h3);
			if (x2 < ((double)x3 + w3) && x2 + w2 > x3 && y2 < ((double)y3 + h3) && y2 + h2 > y3) {
				i->SetVel(1, 0);
				i->SetVel(0, 0);
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
		i -> AddFrame(delta_time_ / fps_desired_);
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
}
void App::AddProjectile(const size_t& index, const int& x, const int& y,double speed, double dir) {
	Projectile* temp = new Projectile(textures_[index], x, y,speed,dir);
	to_render_.push_back(temp);
	projectiles_.push_back(temp);
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
	for (unsigned i = 0; i < 4; i++) {
		temp_room.dir[i] = ((path[name.length() - 5 - i]) == '1');
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
					path_tiles_[unsigned((floor(y / 32) + j) * room_width_ / 32 + (floor(x / 32) + i))] = true;
				break;
				case 77:
					monsters_.push_back(new Monster(textures_[3], i * 32 + x, j * 32 + y));
				break;
				case 80:
					AddWall(2, i * 32 + x, j * 32 + y);
				break;
				case 87:
					AddWall(1, i * 32 + x, j * 32 + y);
				break;
				}

				// Mark the tiles as used, can't generate on top.
				tiles_[unsigned((floor(y / 32) + j) * room_width_ / 32 + (floor(x / 32) + i))] = true;
			}
		}
	}

	return true;
}