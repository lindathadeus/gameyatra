import bindbc.sdl;

import game;

void main(string[] argv) {
  SDLSupport ret = loadSDL();
  
  if (ret != sdlSupport) {
    import std.stdio;
    writeln("Error initializing SDL.");
  } else {
    game_start();
  }
}
