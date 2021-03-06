#include "update.h"

// Pathfinding functions and monster movement updating


namespace monster {

unsigned int calculateHCost(int current, int target, unsigned int w) {
	unsigned int x_diff = abs(int(target % w) - int(current % w));
	unsigned int y_diff = abs((int)floor(target / w) - (int)floor(current / w));
	unsigned int h_cost = std::min(x_diff, y_diff) * 14 + (std::max(x_diff, y_diff) - std::min(x_diff, y_diff)) * 10;
	return h_cost;
}

bool nodeComp(const LocationNode* lhs, const LocationNode* rhs) {
	return lhs->F < rhs->F;
}

std::list<int> getNeighbours(int current_node, bool* path_tiles, unsigned int w, unsigned int size)
{
	std::list<int> neighbours;
	unsigned int original_row = (unsigned int)floor(current_node / w);

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int current_tile = current_node + i * w + j;
			if (current_tile >= 0 && (unsigned)current_tile < size
				&& (int)((original_row+i)*w) <= current_tile && current_tile < (int)((original_row + i + 1)*w) ) {
				if (current_node == current_tile) {
					continue;
				}
				if (path_tiles == nullptr || (path_tiles != nullptr && path_tiles[current_tile]
					&& !( // Checks for corners
						((i == 1 && j == 1)  && (((current_node + 1 < (int)size) && !path_tiles[current_node + 1]) || ((current_node + w < (int)size) && !path_tiles[current_node + w])))
						|| ((i == 1 && j == -1)  && (((current_node - 1 >= 0) && !path_tiles[current_node - 1]) || ((current_node + w < (int)size) && !path_tiles[current_node + w])))
						|| ((i == -1 && j == 1)  && (((current_node + 1 < (int)size) && !path_tiles[current_node + 1]) || ((current_node - w >= 0) && !path_tiles[current_node - w])))
						|| ((i == -1 && j == -1) && (((current_node - 1 >= 0) && !path_tiles[current_node - 1]) || ((current_node - w >= 0) && !path_tiles[current_node - w])))
						))) {
					neighbours.push_back(current_tile);
				}
			}
		}
	}
	
	return neighbours;
}

// Checks whether there are obstacles between the monster and the player
bool getLineOfSight(bool* path_tiles, int current, int target, unsigned int w, unsigned int size) {
	if (path_tiles == nullptr)
		return false;

	int x_diff = int(target % w) - int(current % w);
	int y_diff = (int)floor(target / w) - (int)floor(current / w);

	for (int x = 0; x <= abs(x_diff); x++) {
		for (int y = 0; y <= abs(y_diff); y++) {
			int current_tile = current + (x_diff >= 0 ? x : -x) + (y_diff >= 0 ? y : -y) * w;
			if (0 <= current_tile && current_tile < (int)size) {
				if (!path_tiles[current_tile]) {
					return false;
				}
			}
		}
	}
	if (std::sqrt(x_diff * x_diff + y_diff * y_diff) > 10) {
		return false;
	}
	else {
		return true;
	}	
}

// Main pathfinding algorithm for monsters
void A_star_algorithm(std::shared_ptr<Monster> monster, bool* path_tiles,
	double player_x, double player_y,
	unsigned int size, unsigned int room_width) {
	unsigned int w = room_width / 32;

	double current_x, current_y;
	monster->GetPos(current_x, current_y);

	int target_location = (int)floor(player_y / 32) * w + (int)floor(player_x / 32);
	int current_location = (int)floor(current_y / 32) * w + (int)floor(current_x / 32);

	unsigned int h_start = calculateHCost(current_location, target_location, w);

	if (target_location == current_location) {
		return;
	}

	std::list<LocationNode*> open_nodes;
	std::unordered_map<int, LocationNode*> closed_nodes;

	LocationNode* starting_node = new LocationNode(current_location, h_start);
	starting_node->G = 0;
	starting_node->F = starting_node->H;

	open_nodes.push_back(starting_node);

	LocationNode* last_node = nullptr;

	while (!open_nodes.empty() && closed_nodes.size() < 300) {
		std::list<LocationNode*>::iterator small = std::min_element(open_nodes.begin(), open_nodes.end(), nodeComp);
		if (small == open_nodes.end()) {
			break;
		}
		LocationNode* smallest = *small;
		open_nodes.remove(smallest);
		closed_nodes.insert({smallest->location, smallest});

		if (smallest->location == target_location) {
			last_node = smallest;
			break;
		}

		std::list<int> neighbours = getNeighbours(smallest->location, path_tiles, w, size);

		for (int neighbour : neighbours) {

			auto closed_it = closed_nodes.find(neighbour);
			if (closed_it != closed_nodes.end()) {
				continue;
			}
			bool is_next_to = (int(neighbour % w) == int(smallest->location % w)) || (floor(neighbour / w) == floor(smallest->location / w));
			unsigned int G_add = is_next_to ? 10 : 14;
			LocationNode* neighbour_node;
			auto open_it = std::find_if(open_nodes.begin(), open_nodes.end(), 
				[&](const LocationNode* n) { return n->location == neighbour; });
			if (open_it == open_nodes.end()) {
				neighbour_node = new LocationNode(neighbour, calculateHCost(neighbour, target_location, w));
				open_nodes.push_back(neighbour_node);
			}
			else {
				neighbour_node = (*open_it);
			}
			if ( neighbour_node->G > smallest->G + G_add) {
		 		neighbour_node->G = smallest->G + G_add;
				neighbour_node->F = neighbour_node->G + neighbour_node->H;
				neighbour_node->parent = smallest;
			}
		}
	}
	std::list<int> monster_next_moves;
	if (last_node != nullptr) {
		monster->SetLignOfSight(
			getLineOfSight(path_tiles, current_location, target_location, w, size)
		);
		while(true)
		{
			if (last_node->parent != nullptr) {
				monster_next_moves.push_front(last_node->location);
				last_node = last_node->parent;
			}
			else {
				break;
			}
		};
	}
	for (auto node : open_nodes) {
		delete node;
	}
	open_nodes.clear();
	for (auto node : closed_nodes) {
		delete (node.second);
	}
	closed_nodes.clear();
	monster->ChangeNextMoves(monster_next_moves);
}

void CalculatePath(std::vector<std::shared_ptr<Monster>>& monsters, bool* path_tiles, double player_x, double player_y,
	unsigned size, unsigned int room_width) {
	for (auto& monster : monsters) {
		if (monster->IsDead())
			continue;
		if (!monster->IgnoreWalls()) {
			A_star_algorithm(monster, path_tiles, player_x, player_y, size, room_width);
		}
		else {
			A_star_algorithm(monster, nullptr, player_x, player_y, size, room_width);
		}
		
	}
}

// Moves monsters using the movements stored in monsters generated by A* algorithm
void UpdateMonsters(std::vector<std::shared_ptr<Monster>>& monsters, double delta_speed, const size_t fps, bool can_move,
	unsigned int room_width, std::vector<std::shared_ptr<Wall>> walls, const Texture& tombstone, std::list<std::shared_ptr<Renderable>>& to_render) {

	for (auto& monster : monsters) {
		if (monster->Dead()) { continue; }
		
		if (monster->IsDead()) {
			monster->Kill();
			monster->SetTexture(tombstone);

			if (!to_render.empty()) {
				to_render.erase(remove(to_render.begin(), to_render.end(), monster), to_render.end());
				to_render.push_front(monster);
			}
			monster->AddVel(0, 0);
			monster->CalcPos(fps);

			continue;
		}
		
		if (can_move) {
			monster->PlayerMoved();
		}
		if (!monster->CanMove()) {
			continue;
		}

		if (monster->HasLignOfSight() && !monster->IsMelee()) { continue; }

		double speed = monster->GetSpeed() / fps;

		double current_x, current_y;
		monster->GetPos(current_x, current_y);

		int next_tile = monster->GetNextTile();

		unsigned int w = room_width / 32;
		int current_tile = (int)floor(current_y / 32) * w + (int)floor(current_x / 32);

		if (next_tile == current_tile) {
			monster->PopNextMove();
			next_tile = monster->GetNextTile();
		}
		else if (next_tile != -1) {
			int next_x = (next_tile % w) * 32;
			int next_y = (int)floor(next_tile / w) * 32;

			double speed_x = (next_x - current_x)*2;
			double speed_y = (next_y - current_y)*2;
			double t = sqrt(speed_x * speed_x + speed_y * speed_y);
			speed_x /= t;
			speed_y /= t;
			speed_x *= speed;
			speed_y *= speed;
			monster->AddVel(speed_x, speed_y);
		}
		double x1, y1;
		int w1, h1;
		monster->GetPos(x1, y1);
		monster->GetRect(w1, h1);

		// If cannot monster cannot ignore walls, will check for collision with walls
		if (!monster->IgnoreWalls()) {
			for (auto& i : walls) {
				int x2, y2;
				int w2, h2;
				i->GetPos(x2, y2);
				i->GetRect(w2, h2);

				if (x1 < ((double)x2 + w2) && x1 + w1 > x2 && y1 < ((double)y2 + h2) && y1 + h1 > y2) {
					double x_res = (x1 - x2);
					x_res = x_res + (x_res < 0 ? w2 : -w2);

					double y_res = (y1 - y2);
					y_res = y_res + (y_res < 0 ? h2 : -h2);
					
					if (fabs(x_res) >= 4.0) {
						if (fabs(y_res) > 0.0) {
							monster->AddPos(0, -y_res);
							monster->SetVel(1, 0.0);
						}
					}
					else if (fabs(y_res) >= 4.0) {
						if (fabs(x_res) > 0.0) {
							monster->AddPos(-x_res, 0);
							monster->SetVel(0, 0.0);
						}
					}
				}
			}
		}
		// Cannot stack with other monsters
		for (auto& i : monsters) {
			if (i == monster || i->Dead()) { continue; }
			double x2, y2;
			int w2, h2;
			i->GetPos(x2, y2);
			i->GetRect(w2, h2);

			if (x1 < ((double)x2 + w2) && x1 + w1 > x2 && y1 < ((double)y2 + h2) && y1 + h1 > y2) {
				double x_res = (x1 - x2);
				x_res = x_res + (x_res < 0 ? w2 : -w2);

				double y_res = (y1 - y2);
				y_res = y_res + (y_res < 0 ? h2 : -h2);

				if (fabs(x_res) >= 4.0) {
					if (fabs(y_res) > 0.0) {
						monster->AddPos(0, -y_res);
						monster->SetVel(1, 0.0);
					}
				}
				else if (fabs(y_res) >= 4.0) {
					if (fabs(x_res) > 0.0) {
						monster->AddPos(-x_res, 0);
						monster->SetVel(0, 0.0);
					}
				}
			}
		}
		monster->CalcPos(fps);
	}
}

} // namespace monster
