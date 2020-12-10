# Dungeon 3 

Dungeon 3 is a dungeon crawler game, where you progress through levels and try not to die in progress. To help you survive longer, you should use items provided while progressing the game. Good luck!


## Compiling instructions

Debian
Install CMake and git.
Run `run.sh` with sudo permissions, this will install required packages which are `SDL2`, `SDL2_image`, `SDL2_mixer` and `SDL2_ttf`, download SDL2 Cmake dependencies and compile the game.
This will create `dungeon` binary of the game.


Windows:
Install Git, CMake to path and Visual Studio 16 2019.
Run `run.bat` to do the same, this will generate executable in build/Debug which can be used to run the game.



## Manual instructions Linux
Install git and cmake with package manager
```
apt-get install git
apt-get install cmake
```

To compile the game manually install required SDL packages using the package manager
```
apt-get install libsdl2-dev
apt-get install libsdl2-image-dev
apt-get install libsdl2-ttf-dev
apt-get install libsdl2-mixer-dev
```

Get Cmake for SDL2
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

## Manual instructions Windows
Install Git, CMake to PATH and Visual Studio 16 2019. 

Get Cmake for SDL2 in project root folder `dungeon-2020-3\`
```
git clone https://gitlab.com/aminosbh/sdl2-cmake-modules.git cmake/sdl2
```
Make build directory and use cmake and make to compile the game
```
mkdir build
cd build
cmake ..
```

Compile the game
```
cmake --build . --target
```