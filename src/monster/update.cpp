#include "update.h"


namespace update {

	unsigned int calculateHCost(int current, int target, unsigned int w) {
		unsigned int x_diff = abs(int(target % w) - int(current % w));
		unsigned int y_diff = abs((int)floor(target / w) - (int)floor(current / w));
		unsigned int h_cost = std::min(x_diff, y_diff) * 14 + (std::max(x_diff, y_diff) - std::min(x_diff, y_diff)) * 10;
		return h_cost;
	}

	bool nodeComp(const LocationNode* lhs, const LocationNode* rhs) {
		return lhs->F < rhs->F;
	}

	list<LocationNode*> getNeighbours(LocationNode* current_node, bool* path_tiles, unsigned int w, unsigned int size)
	{
		list<LocationNode*> neighbours;
		unsigned int original_row = (unsigned int)floor(current_node->location / w);

		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				int current_tile = current_node->location + i * w + j;
				unsigned int current_row = (int)floor(current_tile / w);
				if (current_tile >= 0 && (unsigned)current_tile < size
					&& (original_row+i)*w <= current_tile && current_tile < (original_row + i + 1)*w ) {
					if (current_node->location != current_tile && path_tiles[current_tile]) {
						unsigned int h_cost = calculateHCost(current_node->location, current_tile, w);
						neighbours.push_back(new LocationNode(current_tile, h_cost));
					}
				}
			}
		}
		return neighbours;
	}

	void A_star_algorithm(Monster* monster, bool* path_tiles,
		int player_x, int player_y,
		unsigned int size, unsigned int room_width, unsigned int room_height) {
		unsigned int w = room_width / 32;
		unsigned int h = room_height / 32;

		int current_x, current_y;
		monster->GetRect(current_x, current_y);

		int target_location = (int)floor(player_x / 32) * w + (int)floor(player_x / 32);
		int current_location = (int)floor(current_y / 32) * w + (int)floor(current_x / 32);

		unsigned int h_start = calculateHCost(current_location, target_location, w);

		if (target_location == current_location) {
			return;
		}

		std::list<LocationNode*> open_nodes;
		std::list<LocationNode*> closed_nodes;

		LocationNode* starting_node = new LocationNode(current_location, h_start);
		starting_node->G = 0;
		starting_node->F = starting_node->H;

		open_nodes.push_back(starting_node);

		LocationNode* last_node = nullptr;

		while (!open_nodes.empty()) {
			std::list<LocationNode*>::iterator smallest = std::min_element(open_nodes.begin(), open_nodes.end(), nodeComp);
			open_nodes.remove(*smallest);
			closed_nodes.push_back(*smallest);

			if ((*smallest)->location == target_location) {
				last_node = *smallest;
				break;
			}

			std::list<LocationNode*> neighbours = getNeighbours(*smallest, path_tiles, w, size);

			for (auto neighbour : neighbours)
			{
				if (std::find(closed_nodes.begin(), closed_nodes.end(), neighbour) != closed_nodes.end()) {
					continue;
					bool is_next_to = int(neighbour->location % w) == int((*smallest)->location % w)
						|| floor(neighbour->location / w) == floor((*smallest)->location / w);
					unsigned int G_add = is_next_to ? 10 : 14;
					bool neighbour_not_in_open = std::find(open_nodes.begin(), open_nodes.end(), neighbour) == open_nodes.end();
					if (neighbour_not_in_open || neighbour->G > (*smallest)->G + G_add) {
						neighbour->G = (*smallest)->G + G_add;
						neighbour->F = neighbour->G + neighbour->H;
						neighbour->parent = (*smallest);
						if (neighbour_not_in_open) {
							open_nodes.push_back(neighbour);
						}
					}
				}
			}
		}
		std::list<int> monster_next_moves;
		if (last_node != nullptr) {
			do
			{
				monster_next_moves.push_back(last_node->location);
				last_node = last_node->parent;
			} while (last_node->parent != nullptr);
		}
		for (auto node : open_nodes) {
			delete node;
		}
		for (auto node : closed_nodes) {
			delete node;
		}
		monster->ChangeNextMoves(monster_next_moves);
	}

	void CalculatePath(std::vector<Monster*> monsters, bool* path_tiles, int player_x, int player_y,
		unsigned size, unsigned int room_width, unsigned int room_height) {
		for (auto& monster : monsters) {
			A_star_algorithm(monster, path_tiles, player_x, player_y, size, room_width, room_height);
		}
	}

	void UpdateMonsters(std::vector<Monster*>& monsters, double delta_speed, bool can_move,
		unsigned int room_width, unsigned int room_height) {


		for (auto& monster : monsters) {
			if (can_move) {
				monster->PlayerMoved();
			}
			double speed = delta_speed * monster->GetSpeed();
			double current_x, current_y;
			monster->GetPos(current_x, current_y);

			int next_tile = monster->GetNextTile();

			int current_tile = (int)floor(current_y / 32) * (room_width / 32) + (int)floor(current_x / 32);
			if (next_tile == current_tile) {
				monster->PopNextMove();
				next_tile = monster->GetNextTile();
			}

			if (next_tile != -1) {
				int next_x = (next_tile % (room_width / 32)) * 32;
				int next_y = (int)floor(next_tile / (room_width / 32)) * 32;

				double speed_x = (next_x - current_x);
				double speed_y = (next_y - current_y);
				double t = (speed_x * speed_x + speed_y * speed_y);
				speed_x /= t;
				speed_y /= t;
				speed_x *= speed;
				speed_y *= speed;

				monster->AddVel(speed_x, speed_y);
			}
		}
	}

} // namespace UPDATE_H_
