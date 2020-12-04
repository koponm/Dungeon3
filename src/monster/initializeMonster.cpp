#include "initializeMonster.h"

namespace monster {

	void AddMonster(std::vector<std::shared_ptr<Monster>>& monsters, TextureHandler* texture,
		double x, double y, unsigned int difficulty, MonsterType type)
	{
		if (type == MonsterType::MONSTER_COUNT) {
			type = static_cast<MonsterType>(rand() % (int)(MonsterType::MONSTER_COUNT));
		}
		std::shared_ptr<Monster> m = nullptr;
		switch (type) { //get index for texture
		case MonsterType::zombie:
			m = std::shared_ptr<Monster>(new Zombie(texture->Get(TextureType::zombie), x, y, difficulty));
			break;
		case MonsterType::ghost:
			m = std::shared_ptr<Monster>(new Ghost(texture->Get(TextureType::ghost), x, y, difficulty));
			break;

		case MonsterType::skeleton_archer:
			m = std::shared_ptr<Monster>(new Skeleton(texture->Get(TextureType::skeletonArcher), x, y, difficulty));
			break;
		case MonsterType::skeleton_mage:
			m = std::shared_ptr<Monster>(new SkeletonMage(texture->Get(TextureType::skeletonMage), x, y, difficulty));
			break;
		default:
			return;
		}
		monsters.push_back(m);
	}

} // namespace monster