# gameyatra
This is a project to learn about game development.

# Idea
The game is a smaller clone of this game https://armorgames.com/play/13213/i-saw-her-standing-there. We create the project, patch by patch, based on our visual observation of the original game. Each patch can be considered as a code block with a feature or bug fix. Over time, these patches come together to form the complete game, making the learning process practical and easy to follow.

## Steps to run in opensuse
sudo zypper in git
sudo zypper in libX11-devel libXrandr-devel libXcursor-devel libXinerama-devel libXi-devel Mesa-libGL1
sudo cp /usr/lib64/libGL.so.1.7.0 /usr/lib64/libGL.so

git clone --depth 1 https://github.com/raysan5/raylib.git raylib
make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED
sudo make install RAYLIB_LIBTYPE=SHARED

cd ~/gameyatra
cd i-saw*
make
./isaw

## Steps to run in ubuntu
sudo apt install build-essential git
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev

git clone --depth 1 https://github.com/raysan5/raylib.git raylib
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED # To make the dynamic shared version.

sudo make install RAYLIB_LIBTYPE=SHARED # Dynamic shared version.
sudo make uninstall RAYLIB_LIBTYPE=SHARED

cd ~/gameyatra
cd i-saw*
make
./isaw
