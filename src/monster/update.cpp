#include "update.h"

namespace update {

	unsigned int calculateHCost(int current, int target, unsigned int w) {
		unsigned int x_diff = abs(int(target % w) - int(current % w));
		unsigned int y_diff = abs((int)floor(target / w) - (int)floor(current / w));
		unsigned int h_cost = min(x_diff, y_diff) * 14 + (max(x_diff, y_diff) - min(x_diff, y_diff)) * 10;
		return h_cost;
	}

	bool nodeComp(const LocationNode* lhs, const LocationNode* rhs) {
		return lhs->F < rhs->F;
	};

	void CalculatePath(vector<Monster*> monsters, bool* path_tiles, int player_x, int player_y,
		unsigned size, unsigned int room_width, unsigned int room_height) {
		for (auto& monster : monsters) {

		}
	}

	void UpdateMonsters(vector<Monster*>& monsters, double delta_speed, bool can_move,
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
				int next_x = (next_tile % (room_height / 32)) * 32;
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