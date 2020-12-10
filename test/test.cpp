#define SDL_MAIN_HANDLED


#include "pch.h"
#include "../src/monster/LocationNode.h"
#include "../src/monster/update.h"

#include "../src/monster/update.cpp"
#include "../src/monster/monster.cpp"
#include "../src/wall.cpp"
#include "../src/renderable.cpp"

#include <list>
#include <algorithm>
#include <memory>


TEST(Pathfinding, calculateHCost) {

    unsigned int w = 5;
    unsigned int res1  = monster::calculateHCost(0, 9, w);
    unsigned int res2 = monster::calculateHCost(5, 4, w);
    EXPECT_EQ(res1, 44);
    EXPECT_EQ(res2, 44);
}

TEST(Pathfinding, nodeComp) {

    std::list<LocationNode*> list;
    LocationNode* node1 = new LocationNode(1, 5);
    LocationNode* node2= new LocationNode(100, 10);
    LocationNode* node3 = new LocationNode(100, 100);
    node1->F = 100;
    node2->F = 10;
    list.push_back(node1);
    list.push_back(node2);
    list.push_back(node3);
    std::list<LocationNode*>::iterator  smallest = std::min_element(list.begin(), list.end(), monster::nodeComp);
    EXPECT_EQ((*smallest)->location, node2->location);

    for (auto node : list) {
        delete node;
    }
    list.clear();
}

TEST(Pathfinding, getNeighbours_left_excluded) {
    unsigned int size = 8 * 8;
    bool* tiles = new bool[size];
    for (unsigned i = 0; i < size; i++) {
        tiles[i] = true;
    }

    std::list<int> list = monster::getNeighbours(0, tiles, 8, size);

    std::list<int> expected_list({1, 8, 9});

    EXPECT_EQ(expected_list, list);
}

TEST(Pathfinding, getNeighbours_right_excluded) {
    unsigned int size = 8 * 8;
    bool* tiles = new bool[size];
    for (unsigned i = 0; i < size; i++) {
        tiles[i] = true;
    }

    std::list<int> list = monster::getNeighbours(7, tiles, 8, size);

    std::list<int> expected_list({ 6, 14, 15 });

    EXPECT_EQ(expected_list, list);
}

TEST(Pathfinding, getNeighbours_center) {
    unsigned int size = 8 * 8;
    bool* tiles = new bool[size];
    for (unsigned i = 0; i < size; i++) {
        tiles[i] = true;
    }
    tiles[33] = false;

    std::list<int> list = monster::getNeighbours(34, tiles, 8, size);
    std::list<int> expected_list({ 26, 27, 35, 42, 43 });

    EXPECT_EQ(expected_list, list);
}

TEST(Pathfinding, getNeighbours_center_2) {
    unsigned int size = 8 * 8;
    bool* tiles = new bool[size];
    for (unsigned i = 0; i < size; i++) {
        tiles[i] = true;
    }
    tiles[26] = false;
    tiles[42] = false;

    std::list<int> list = monster::getNeighbours(34, tiles, 8, size);
    std::list<int> expected_list({ 33, 35 });

    EXPECT_EQ(expected_list, list);
}

TEST(Pathfinding, getNeighbours_false) {
    unsigned int size = 8 * 8;
    bool* tiles = new bool[size];
    for (unsigned i = 0; i < size; i++) {
        tiles[i] = false;
    }

    std::list<int> list = monster::getNeighbours(1, tiles, 8, size);

    EXPECT_TRUE(list.empty());
}

TEST(Pathfinding, A_star_algorithm_1) {
    Texture text;
    std::shared_ptr<Monster> monster(new Monster(text, 1 * 32, 1 * 32, MonsterType::MONSTER_COUNT));
    unsigned int size = 8 * 8;
    bool* tiles = new bool[size];
    for (unsigned i = 0; i < size; i++) {
        tiles[i] = true;
    }
    double x1, x2;
    monster->GetPos(x1, x2);
    EXPECT_EQ(x1, 32.0);
    monster::A_star_algorithm(monster, tiles, 2*32, 1*32, size, 8 * 32, 8 * 32);
    EXPECT_EQ(monster->GetNextTile(), 10);

}

TEST(Pathfinding, A_star_algorithm_2) {
    Texture text;
    std::shared_ptr<Monster> monster(new Monster(text, 1 * 32, 1 * 32, MonsterType::MONSTER_COUNT));
    unsigned int size = 8 * 8;
    bool* tiles = new bool[size];
    for (unsigned i = 0; i < size; i++) {
        tiles[i] = true;
    }
    double x1, x2;
    monster->GetPos(x1, x2);
    EXPECT_EQ(x1, 32.0);
    monster::A_star_algorithm(monster, tiles, 3 * 32, 1 * 32, size, 8 * 32, 8 * 32);
    EXPECT_EQ(monster->GetNextTile(), 10);
    monster->PopNextMove();
    EXPECT_EQ(monster->GetNextTile(), 11);
    monster->PopNextMove();
    EXPECT_EQ(monster->GetNextTile(), -1);

}

TEST(Pathfinding, A_star_algorithm_3) {
    Texture text;
    std::shared_ptr<Monster> monster(new Monster(text, 1 * 32, 1 * 32, MonsterType::MONSTER_COUNT));
    unsigned int size = 8 * 8;
    bool* tiles = new bool[size];
    for (unsigned i = 0; i < size; i++) {
        tiles[i] = true;
    }
    tiles[4] = false;
    tiles[11] = false;
    tiles[12] = false;
    tiles[27] = false;
    tiles[28] = false;
    double x1, x2;
    monster->GetPos(x1, x2);
    EXPECT_EQ(x1, 32.0);
    monster::A_star_algorithm(monster, tiles, 5 * 32, 0 * 32, size, 8 * 32, 8 * 32);
    for (unsigned int i = 0; i < 5; i++)
        monster->PopNextMove();
    EXPECT_EQ(monster->GetNextTile(), 5);

}


