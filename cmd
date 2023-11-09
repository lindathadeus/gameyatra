## DIY steps to create the game yourself instead of cloning from this repo

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
