project(dungeon)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

# Add SDL2 CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

file(GLOB SOURCES
	"src/*.h"
	"src/*.cpp"
	"src/monster/*.h"
	"src/monster/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})


find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::Main)
find_package(SDL2_image REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::Image)
find_package(SDL2_ttf REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::TTF)
find_package(SDL2_mixer REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::Mixer)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/assets DESTINATION ${CMAKE_BINARY_DIR})




