import bindbc.sdl;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PADDLE_WIDTH = 20;
const int PADDLE_HEIGHT = 100;
const int BALL_SIZE = 20;

struct Paddle {
    SDL_Rect rect;
    int dy;
}

struct Ball {
    SDL_Rect rect;
    int dx, dy;
}

void main() {
    if (loadSDL()) {
        SDL_Init(SDL_INIT_VIDEO);

        SDL_Window* window = SDL_CreateWindow("PongGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        Paddle paddleA = {{0, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT}, 0};
        Paddle paddleB = {{SCREEN_WIDTH - PADDLE_WIDTH, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT}, 0};
        Ball ball = {{SCREEN_WIDTH / 2 - BALL_SIZE / 2, SCREEN_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE, BALL_SIZE}, 1, 1};

        bool running = true;
        SDL_Event e;

        while (running) {
            while (SDL_PollEvent(&e) != 0) {
                switch (e.type) {
                    case SDL_QUIT:
                        running = false;
                        break;
                    case SDL_KEYDOWN:
                        switch (e.key.keysym.sym) {
                            case SDLK_w:
                                paddleA.dy = -1;
                                break;
                            case SDLK_s:
                                paddleA.dy = 1;
                                break;
                            case SDLK_UP:
                                paddleB.dy = -1;
                                break;
                            case SDLK_DOWN:
                                paddleB.dy = 1;
                                break;
                        }
                        break;
                    case SDL_KEYUP:
                        switch (e.key.keysym.sym) {
                            case SDLK_w:
                            case SDLK_s:
                                paddleA.dy = 0;
                                break;
                            case SDLK_UP:
                            case SDLK_DOWN:
                                paddleB.dy = 0;
                                break;
                        }
                        break;
                }
            }

            paddleA.rect.y += paddleA.dy;
            paddleB.rect.y += paddleB.dy;
            ball.rect.x += ball.dx;
            ball.rect.y += ball.dy;

            if (ball.rect.y <= 0 || ball.rect.y + BALL_SIZE >= SCREEN_HEIGHT) {
                ball.dy = -ball.dy;
            }

            SDL_Rect paddleACollision = SDL_Rect(paddleA.rect.x, paddleA.rect.y, paddleA.rect.w, paddleA.rect.h);
            SDL_Rect paddleBCollision = SDL_Rect(paddleB.rect.x, paddleB.rect.y, paddleB.rect.w, paddleB.rect.h);

            if (SDL_HasIntersection(&ball.rect, &paddleACollision) || SDL_HasIntersection(&ball.rect, &paddleBCollision)) {
                ball.dx = -ball.dx;
            }

            if (ball.rect.x <= 0) {
                ball.rect.x = SCREEN_WIDTH / 2 - BALL_SIZE / 2;
                ball.rect.y = SCREEN_HEIGHT / 2 - BALL_SIZE / 2;
                ball.dx = 1;
            }

            if (ball.rect.x + BALL_SIZE >= SCREEN_WIDTH) {
                ball.rect.x = SCREEN_WIDTH / 2 - BALL_SIZE / 2;
                ball.rect.y = SCREEN_HEIGHT / 2 - BALL_SIZE / 2;
                ball.dx = -1;
            }

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &paddleA.rect);
            SDL_RenderFillRect(renderer, &paddleB.rect);
            SDL_RenderFillRect(renderer, &ball.rect);

            SDL_RenderPresent(renderer);
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    } else {
        import std.stdio;
        writeln("Error initializing SDL.");
    }
}
