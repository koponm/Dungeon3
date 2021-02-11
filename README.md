# Dungeon 3 

Dungeon 3 is a dungeon crawler game, where you progress through levels and try not to die in progress. To help you survive longer, you should use items provided while progressing the game. Good luck!


## Compiling instructions

Linux:  
Install git, CMake and a compiler.  
Run `run.sh` with sudo permissions, this will install required packages which are `SDL2`, `SDL2_image`, `SDL2_mixer` and `SDL2_ttf`, download SDL2 CMake dependencies and compile the game.
This will create `dungeon` binary of the game at `build/dungeon`.


Windows:  
To compile the game, you will need to get the following dll files:  
SDL2.dll  
SDL2_image.dll  
SDL2_mixer.dll  
SDL2_ttf.dll  
libfreetype-6.dll  
libogg-0.dll  
libpng16-16.dll  
libvorbis-0.dll  
libvorbisfile-3.dll  
zlib1.dll

and include the following libraries  
SDL2  
SDL2_image  
SDL2_mixer  
SDL2_ttf

You can compile using Visual Studio or minGW.
If using cmake, you will need cmake dependencies, found at
https://gitlab.com/aminosbh/sdl2-cmake-modules

## Manual instructions for Linux
Install git, cmake and compiler with package manager
```
apt-get install git
apt-get install cmake
apt-get install build-essential
```

Install required SDL packages using the package manager
```
apt-get install libsdl2-dev
apt-get install libsdl2-image-dev
apt-get install libsdl2-ttf-dev
apt-get install libsdl2-mixer-dev
```

Get CMake support for SDL2
```
git clone https://gitlab.com/aminosbh/sdl2-cmake-modules.git cmake/sdl2
```

Make build directory and use cmake and make to compile the game
```
mkdir build
cd build
cmake ..
make
```

Run the game with
```
./dungeon
```
