#include "initializeMonster.h"

namespace monster {

	void AddMonster(std::vector<std::shared_ptr<Monster>>& monsters, TextureHandler* texture,
		double x, double y, unsigned int difficulty, MonsterType type)
	{
		if (type == MonsterType::MONSTER_COUNT) {
			type = static_cast<MonsterType>(rand() % (int)(MonsterType::MONSTER_COUNT));
		}
		std::shared_ptr<Monster> m = nullptr;
		TextureType ttype;
		auto roll = rand() % 2;
		switch (type) { //get index for texture
		case MonsterType::zombie:
			ttype = zombie;
			if (difficulty > 4 && roll) {
				ttype = superzombie;
			}
			m = std::shared_ptr<Monster>(new Zombie(texture->Get(ttype), x, y, difficulty + (roll + 1)));
			break;
		case MonsterType::ghost:
			ttype = ghost;
			if (difficulty > 4 && roll) {
				ttype = banshee;
			}
			m = std::shared_ptr<Monster>(new Ghost(texture->Get(ttype), x, y, difficulty + (roll + 1)));
			break;

		case MonsterType::skeleton_archer:
			ttype = skeletonArcher;
			if (difficulty > 4 && roll) {
				ttype = superarcher;
			}
			m = std::shared_ptr<Monster>(new Skeleton(texture->Get(ttype), x, y, difficulty + (roll + 1)));
			break;
		case MonsterType::skeleton_mage:
			ttype = skeletonMage;
			if (difficulty > 4 && roll) {
				ttype = lich;
			}
			m = std::shared_ptr<Monster>(new SkeletonMage(texture->Get(ttype), x, y, difficulty + (roll + 1)));
			break;
		default:
			return;
		}
		monsters.push_back(m);
	}

} // namespace monster