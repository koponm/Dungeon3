cmake_minimum_required(VERSION 3.10)

project(dungeon)


file(GLOB SOURCES
	"src/*.h"
	"src/*.cpp"
	"src/monster/*.h"
	"src/monster/*.cpp"
	"src/projectile/*.h"
	"src/projectile/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})


#  LINKING SDL2
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/sdl2")

if (WIN32)
set(SDL2_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2" CACHE BOOL "" FORCE)
set(SDL2_IMAGE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/" CACHE BOOL "" FORCE)
set(SDL2_TTF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/" CACHE BOOL "" FORCE)
set(SDL2_MIXER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/" CACHE BOOL "" FORCE)
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image SDL2::TTF SDL2::Mixer)

# Copy assets
if(WIN32)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/assets DESTINATION ${CMAKE_BINARY_DIR}/Debug)
else()
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/assets DESTINATION ${CMAKE_BINARY_DIR})
endif()

if(WIN32)
# Copy DLL files to output directory
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/lib/x64/SDL2.dll" 		${CMAKE_BINARY_DIR}/Debug
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/lib/x64/SDL2_image.dll"		${CMAKE_BINARY_DIR}/Debug
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/lib/x64/SDL2_ttf.dll"		${CMAKE_BINARY_DIR}/Debug
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/lib/x64/SDL2_mixer.dll"		${CMAKE_BINARY_DIR}/Debug
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/lib/x64/libfreetype-6.dll" 	${CMAKE_BINARY_DIR}/Debug
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/lib/x64/libogg-0.dll" 		${CMAKE_BINARY_DIR}/Debug
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/lib/x64/libpng16-16.dll" 	${CMAKE_BINARY_DIR}/Debug
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/lib/x64/libvorbis-0.dll" 	${CMAKE_BINARY_DIR}/Debug
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/lib/x64/libvorbisfile-3.dll" 	${CMAKE_BINARY_DIR}/Debug
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/lib/x64/zlib1.dll" 		${CMAKE_BINARY_DIR}/Debug

	COMMENT "Copying SDL2 binaries" VERBATIM
)
endif()