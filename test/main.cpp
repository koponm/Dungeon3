#include "SDL.h"
#include "pch.h"
#include "../src/app.h"
#include <gtest/gtest.h>

int main(int argc, char* argv[]) {
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}