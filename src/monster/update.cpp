#include "update.h"

void UpdateMonsters(vector<Monster*>& monsters, size_t fps, double speed, vector<Wall*>& walls, bool can_move) {


	for (auto& monster : monsters) {
		if (can_move) {
			monster->PlayerMoved();
		}

		if (monster->CanMove()) {
			speed = speed * 2;
			int selection = rand() % 4 + 1;
			switch (selection)
			{
			case 1:
				monster->AddVel(0, speed);
				break;
			case 2:
				monster->AddVel(0, -speed);
				break;
			case 3:
				monster->AddVel(speed, 0);
				break;
			case 4:
				monster->AddVel(-speed, 0);
				break;
			}
			
		}

		double previous_x, previous_y;
		monster->GetPos(previous_x, previous_y);
		monster->CalcPos(fps);

		for (auto& i : walls) {
			double x1, y1;
			int x2, y2;
			monster->GetPos(x1, y1);
			i->GetPos(x2, y2);
			if (x1 < ((double)x2 + 32) && x1 + 32 > x2 && y1 < ((double)y2 + 32) && y1 + 32 > y2) {
				double x_res = (x1 - x2);
				x_res = x_res + (x_res < 0 ? 32 : -32);

				double y_res = (y1 - y2);
				y_res = y_res + (y_res < 0 ? 32 : -32);

				if (fabs(x_res) >= 1.0) {
					if (fabs(y_res) > 0.0) {
						monster->AddPos(0, -y_res);
						monster->SetVel(1, 0.0);
					}
				}
				else if (fabs(y_res) >= 1.0) {
					if (fabs(x_res) > 0.0) {
						monster->AddPos(-x_res, 0);
						monster->SetVel(0, 0.0);
					}
				}
			}
		}
	}
}