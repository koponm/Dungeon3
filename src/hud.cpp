#include "hud.h"

HUD_object::HUD_object(const Texture& texture, int x, int y, ItemType type, int frame) : Renderable(texture) {
	x_ = x;
	y_ = y;
	type_ = type;
	RectPos(x_, y_);
	SetImageSpeed(0);
	SetFrame(frame);
}

void HUD_object::UpdateHUD(Player* player) {
	switch (type_) {
	case ItemType::health_potion:
		if (player->GetHealthPotions() > 0) {
			SetFrame(0);
		}
		else SetFrame(1);
		break;
	case ItemType::mana_potion:
		if (player->GetManaPotions() > 0) {
			SetFrame(2);
		}
		else SetFrame(3);
		break;
	case ItemType::sword:
		ItemType temp = player->GetWeapon();
		if (temp == ItemType::staff) SetFrame(4);
		else SetFrame(5);
		break;
	}
}