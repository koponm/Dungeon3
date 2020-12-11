#!/bin/sh


# Update and install compiler
apt-get update
apt-get install build-essential

# Install SDL2
apt-get install libsdl2-dev
apt-get install libsdl2-image-dev
apt-get install libsdl2-ttf-dev
apt-get install libsdl2-mixer-dev

# Download cmake for SDL2
git clone https://gitlab.com/aminosbh/sdl2-cmake-modules.git cmake/sdl2
rm -rf cmake/sdl2/.git

# Create build directory
rm -rf build
mkdir build
cd build

# Execute cmake
cmake ..

# Compile with make
make
