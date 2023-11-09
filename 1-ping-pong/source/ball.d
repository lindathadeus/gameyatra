module ball;

import bindbc.sdl;

struct Ball {
  SDL_Rect rect;
  int dx, dy;
}
