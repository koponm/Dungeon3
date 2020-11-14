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

	std::list<int> getNeighbours(int current_node, bool* path_tiles, unsigned int w, unsigned int size)
	{
		std::list<int> neighbours;
		unsigned int original_row = (unsigned int)floor(current_node / w);

		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				int current_tile = current_node + i * w + j;
				unsigned int current_row = (int)floor(current_tile / w);
				if (current_tile >= 0 && (unsigned)current_tile < size
					&& (int)((original_row+i)*w) <= current_tile && current_tile < (int)((original_row + i + 1)*w) ) {
					if (current_node != current_tile && path_tiles[current_tile]) {
						neighbours.push_back(current_tile);
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

			std::list<int> neighbours = getNeighbours((*smallest)->location, path_tiles, w, size);


			for (int neighbour : neighbours) {
				auto closed_it = std::find_if(closed_nodes.begin(), closed_nodes.end(),
					[&](const LocationNode* n) { return n->location == neighbour; });
				if (closed_it == closed_nodes.end()) {
					continue;
				}
				bool is_next_to = int(neighbour % w) == int((*smallest)->location % w)
					|| floor(neighbour / w) == floor((*smallest)->location / w);
				unsigned int G_add = is_next_to ? 10 : 14;
				LocationNode* neighbour_node;
				auto open_it = std::find_if(open_nodes.begin(), open_nodes.end(), 
					[&](const LocationNode* n) { return n->location == neighbour; });
				if (open_it == open_nodes.end()) {
					neighbour_node = new LocationNode(neighbour, calculateHCost(neighbour, (*smallest)->location, w));
				}
				else {
					neighbour_node = (*open_it);
				}
				if ( neighbour_node->G > (*smallest)->G + G_add) {
		 			neighbour_node->G = (*smallest)->G + G_add;
					neighbour_node->F = neighbour_node->G + neighbour_node->H;
					neighbour_node->parent = (*smallest);
					if (open_it == open_nodes.end()) {
						open_nodes.push_back(neighbour_node);
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
