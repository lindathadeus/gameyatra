## Steps to run 2-i-saw
sudo apt install build-essential git

sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev

git clone --depth 1 https://github.com/raysan5/raylib.git raylib
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED # To make the dynamic shared version.

sudo make install RAYLIB_LIBTYPE=SHARED # Dynamic shared version.

sudo make uninstall RAYLIB_LIBTYPE=SHARED

cd 2-i-saw
make
./isaw

## DIY steps to create the first level game yourself instead of cloning from this repo

# install dmd
curl -fsS https://dlang.org/install.sh | bash -s dmd

# setup PATH, LIBRARY_PATH, LD_LIBRARY_PATH, DMD, DC, and PS1
source ~/dlang/dmd-2.105.3/activate

# restore your environment
deactivate

# dub manager
dub init 1-ping-pong

# add the necessary sdl libs
dub add bindbc-sdl

# build and run
dub build
dub run

# ref
https://wiki.libsdl.org/
