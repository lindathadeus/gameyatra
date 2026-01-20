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
