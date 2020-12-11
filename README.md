# Dungeon 3 

Dungeon 3 is a dungeon crawler game, where you progress through levels and try not to die in progress. To help you survive longer, you should use items provided while progressing the game. Good luck!


## Compiling instructions

Linux:  
Install CMake and git.  
Run `run.sh` with sudo permissions, this will install required packages which are `SDL2`, `SDL2_image`, `SDL2_mixer` and `SDL2_ttf`, download SDL2 CMake dependencies and compile the game.
This will create `dungeon` binary of the game at `build/dungeon`.


Windows:  
Install Visual Studio 16 2019 and open the solution file (.sln).
Select dungeon3 as a startup project and run with windows local debugger. This will start the game and create an executable at x64/Debug or x64/Release.
You could also copy the exe file to `src/` directory to run the game from the executable since all the required .dll files and assets are located there.


## Manual instructions for Linux
Install git and cmake with package manager
```
apt-get install git
apt-get install cmake
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
