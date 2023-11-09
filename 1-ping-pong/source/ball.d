module ball;

import bindbc.sdl;

const int BALL_SIZE = 20;

struct Ball {
  SDL_Rect rect;
  int dx, dy;
}
