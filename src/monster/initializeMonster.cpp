#include "initializeMonster.h"

namespace monster {

	void AddMonster(std::vector<Monster*>& monsters, TextureHandler* texture,
		double x, double y, unsigned int difficulty, MonsterType type)
	{
		if (type == MonsterType::MONSTER_COUNT) {
			type = static_cast<MonsterType>(rand() % (int)(MonsterType::MONSTER_COUNT));
		}
		Monster* m = nullptr;
		switch (type) { //get index for texture
		case MonsterType::zombie:
			m = new Zombie(texture->Get(TextureType::zombie), x, y);
			break;
		case MonsterType::ghost:
			m = new Ghost(texture->Get(TextureType::ghost), x, y);
			break;

		case MonsterType::skeleton_archer:
			m = new Skeleton(texture->Get(TextureType::skeletonArcher), x, y);
			break;
		case MonsterType::skeleton_mage:
			m = new SkeletonMage(texture->Get(TextureType::skeletonMage), x, y);
			break;
		}
		if (m)
			monsters.push_back(m);
	}

} // namespace monster