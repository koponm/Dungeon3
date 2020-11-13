#define SDL_MAIN_HANDLED


#include "pch.h"
#include "../src/monster/LocationNode.h"
#include "../src/monster/update.h"

#include "../src/monster/update.cpp"
#include "../src/monster/monster.cpp"
#include "../src/renderable.cpp"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}


TEST(calculateHcost_test, H_value) {

    int location1 = 0;
    int location2 = 5;
    unsigned int w = 5;
    unsigned int result = 1;
    result  = update::calculateHCost(location1, location2, w);
    EXPECT_EQ(result, 44);
}

