module sdl_handler;

import bindbc.sdl;

import window;

void init_SDL(SDL_Window* window, SDL_Renderer* renderer) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindow("PongGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void cleanup_SDL(SDL_Window* window, SDL_Renderer* renderer) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
