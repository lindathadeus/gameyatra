module paddle;

import bindbc.sdl;

struct Paddle {
  SDL_Rect rect;
  int dy;
}
