@echo off

call git clone https://gitlab.com/aminosbh/sdl2-cmake-modules.git cmake/sdl2

RD /S /Q build
mkdir build
cd build


cmake ..
cmake --build . --target

PAUSE