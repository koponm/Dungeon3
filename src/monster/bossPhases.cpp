#include "bossPhases.h"



void BossHandler::SpawnMobs(unsigned int amount, MonsterType monstertype1, MonsterType monstertype2)
{

	if (path_tiles_ == nullptr || tiles_ == nullptr) { return; }
	double boss_x, boss_y;
	boss_->GetPos(boss_x, boss_y);
	int current_tile = (int)floor(boss_y / 32) * (int)(w_/32) + (int)floor(boss_x / 32);

	std::vector<int> results;

	for (int i = -2; i < 2; i++)
	{
		for (int j = -2; j < 2; j++)
		{
			int test_tile = current_tile + i + j * int(w_ / 32);

			if (0 <= test_tile && test_tile < size_ && test_tile != current_tile &&
				path_tiles_[test_tile] && tiles_[test_tile]) {
				results.push_back(test_tile);
			}
		}
	}
	std::random_shuffle(results.begin(), results.end());
	unsigned count = 0;
	for (int i : results) {
		if (count >= amount) { continue; }
		std::shared_ptr<Monster> monsterptr(monster::AddMonster(
			monsters_, textures_, double(i % (w_ / 32)) * 32, double(i / (w_ / 32)) * 32, difficulty_, 
			(rand() % 2 == 0) ? monstertype1 : monstertype2));
		entities_.push_back(monsterptr);
		count++;
	}	
}

void BossHandler::RessurectMobs()
{
	for (auto monster : monsters_)
	{
		if (monster->Dead()) {
			monster->Ressurect();
			monster->AddVel(0, 0);
			monster->CalcPos(60);
		}
	}
}

void BossHandler::BossPhase()
{
	
	if (boss_->GetHealth() <= 0.75 * boss_->GetMaxHealth() && !stage1)
	{
		stage1 = true;
		if (boss_->GetMonsterType() == MonsterType::banshee) {
			SpawnMobs(2, MonsterType::ghost, MonsterType::ghost);
			boss_->SetHealth(std::min(boss_->GetMaxHealth(), boss_->GetHealth() + 20.0 * (1 + 0.2 * difficulty_)));
		}
		if (boss_->GetMonsterType() == MonsterType::necromancer) {
			SpawnMobs(2, MonsterType::zombie, MonsterType::skeleton_archer);
		}
	}
	else if (boss_->GetHealth() <= 0.5 * boss_->GetMaxHealth() && !stage2)
	{
		stage2 = true;
		if (boss_->GetMonsterType() == MonsterType::banshee) {
			SpawnMobs(3, MonsterType::ghost, MonsterType::ghost);
			boss_->SetHealth(std::min(boss_->GetMaxHealth(), boss_->GetHealth() + 20.0 * (1 + 0.2 * difficulty_)));
		}
		if (boss_->GetMonsterType() == MonsterType::necromancer) {
			SpawnMobs(2, MonsterType::zombie, MonsterType::skeleton_mage);
		}
	}
	else if (boss_->GetHealth() <= 0.25 * boss_->GetMaxHealth() && !stage3)
	{
		stage3 = true;
		if (boss_->GetMonsterType() == MonsterType::banshee) {
			SpawnMobs(2, MonsterType::ghost, MonsterType::skeleton_mage);
		}
		if (boss_->GetMonsterType() == MonsterType::necromancer) {
			RessurectMobs();
		}
	}

}

