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
		window_ = SDL_CreateWindow(title_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, (fullscreen_ ? (int) SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN));
		renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
		SDL_SetRenderDrawColor(renderer_, 32, 32, 32, 255);

		LoadTexture("assets/test.png");
		LoadTexture("assets/wall.png");
		LoadTexture("assets/wood.png");

		player_ = new Player(textures_[0], textures_data_[0], 64, 64);
		to_render_.push_back(player_);

		LoadRoom("assets/room.txt");
		LoadRoom("assets/room2.txt");

		AddRoom(0, 32, 32);
		AddRoom(0, 32, 288);
		AddRoom(1, 288, 224);
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
	double delta_time = (double)((now_ - last_) * 1000.0 / (double)SDL_GetPerformanceFrequency());

	double speed = 2.0 / fps_desired_ * delta_time;

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
	
	double previous_x, previous_y;
	player_ -> GetPos(previous_x, previous_y);

	player_ -> CalcPos(fps_desired_);

	for (auto& i : walls_) {
		double x1, y1;
		int x2, y2;
		player_ -> GetPos(x1, y1);
		i -> GetPos(x2, y2);
		if (x1 < ((double) x2 + 32) && x1 + 32 > x2 && y1 < ((double) y2 + 32) && y1 + 32 > y2) {
			double x_res = (x1 - x2);
			x_res = x_res + (x_res < 0 ? 32 : -32);

			double y_res = (y1 - y2);
			y_res = y_res + (y_res < 0 ? 32 : -32);

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

	player_ -> GetPos(camera_x_, camera_y_);

	double x_offset = width_ / 2.0;
	double y_offset = height_ / 2.0;

	camera_x_ = floor(camera_x_ - x_offset + 0.5 + 16.0);
	camera_y_ = floor(camera_y_ - y_offset + 0.5 + 16.0);

	camera_x_ = min(max(0.0, camera_x_), max(0.0, 1024 - x_offset * 2));
	camera_y_ = min(max(0.0, camera_y_), max(0.0, 1024 - y_offset * 2));
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
			}
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
	Wall* temp = new Wall(textures_[index], textures_data_[index], x, y);
	to_render_.push_back(temp);
	walls_.push_back(temp);
}

void App::LoadTexture(const char* path) {
	SDL_Surface* temp = IMG_Load(path);
	SDL_Rect temp_rect;

	temp_rect.w = temp -> w;
	temp_rect.h = temp -> h;
	temp_rect.x = 0;
	temp_rect.y = 0;

	textures_.push_back(SDL_CreateTextureFromSurface(renderer_, temp));
	textures_data_.push_back(temp_rect);

	SDL_FreeSurface(temp);
}

void App::LoadRoom(const char* path) {
	Room temp_room;
	ifstream file(path);
	string line;
	vector<int> temp;

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

void App::AddRoom(const unsigned int& index, const int& x, const int& y) {
	unsigned w, h;
	w = room_data_[index].width;
	h = room_data_[index].height;

	for (unsigned j = 0; j < h; j++) {
		for (unsigned i = 0; i < w; i++) {
			switch (room_data_[index].data[unsigned (j * h + i)]) {
			default:
			break;
			case 45:
				// Air
			break;
			case 80:
				AddWall(2, i * 32 + x, j * 32 + y);
			break;
			case 87:
				AddWall(1, i * 32 + x, j * 32 + y);
			break;
			}
		}
	}
}