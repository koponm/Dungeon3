#define SDL_MAIN_HANDLED


#include "pch.h"
#include "../src/monster/LocationNode.h"
#include "../src/monster/update.h"

#include "../src/monster/update.cpp"
#include "../src/monster/monster.cpp"
#include "../src/renderable.cpp"

#include <list>
#include <algorithm>

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}


TEST(Pathfinding, calculateHCost) {

    unsigned int w = 5;
    unsigned int res1  = update::calculateHCost(0, 9, w);
    unsigned int res2 = update::calculateHCost(5, 4, w);
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
    std::list<LocationNode*>::iterator  smallest = std::min_element(list.begin(), list.end(), update::nodeComp);
    EXPECT_EQ((*smallest)->location, node2->location);

    for (auto node : list) {
        delete node;
    }
}

TEST(Pathfinding, getNeighbours_left_excluded) {
    unsigned int size = 8 * 8;
    bool* tiles = new bool[size];
    for (unsigned i = 0; i < size; i++) {
        tiles[i] = true;
    }

    std::list<int> list = update::getNeighbours(0, tiles, 8, size);

    std::list<int> expected_list({1, 8, 9});

    EXPECT_EQ(expected_list, list);
}

TEST(Pathfinding, getNeighbours_right_excluded) {
    unsigned int size = 8 * 8;
    bool* tiles = new bool[size];
    for (unsigned i = 0; i < size; i++) {
        tiles[i] = true;
    }

    std::list<int> list = update::getNeighbours(7, tiles, 8, size);

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

    std::list<int> list = update::getNeighbours(34, tiles, 8, size);
    std::list<int> expected_list({ 25, 26, 27, 35, 41, 42, 43 });

    EXPECT_EQ(expected_list, list);
}

TEST(Pathfinding, getNeighbours_false) {
    unsigned int size = 8 * 8;
    bool* tiles = new bool[size];
    for (unsigned i = 0; i < size; i++) {
        tiles[i] = false;
    }

    std::list<int> list = update::getNeighbours(1, tiles, 8, size);

    EXPECT_TRUE(list.empty());
}
