module paddle;

import bindbc.sdl;
import window;

const int PADDLE_WIDTH = 20;
const int PADDLE_HEIGHT = 100;

struct Paddle {
  SDL_Rect rect;
  int dy;
}

Paddle paddleA = {{0, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT}, 0};

Paddle paddleB = {{SCREEN_WIDTH - PADDLE_WIDTH, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT}, 0};
