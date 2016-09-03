#include <SDL2/SDL.h>

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

typedef struct Color {
  int r;
  int g;
  int b;
  int a;
} Color;

typedef struct Point {
  int x;
  int y;
} Point;

typedef struct Pixel {
  Point point;
  Color color;
} Pixel;

void draw(SDL_Window *window, SDL_Renderer *renderer);
void drawPixel(Pixel pixel, SDL_Renderer *renderer);
Pixel randomPixel(int w, int h);

int main(int argc, char *args[]) {
  time_t t;
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  /* Initialize random number generator */
  srand((unsigned) time(&t));

  /* Create SDL Window */
  window = SDL_CreateWindow(
      "SDL Starter",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      DEFAULT_WIDTH,
      DEFAULT_HEIGHT,
      SDL_WINDOW_SHOWN
  );
  if(window == NULL) {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
  }

  /* Create SDL Renderer */
  renderer = SDL_CreateRenderer(
      window,
      -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );
  if(renderer == NULL) {
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
  }
 
  /* SDL Loop */ 
  int fullscreen = 0; 
  while(1) {
    SDL_Event event;
    if(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        break;
      } else if(event.type == SDL_KEYUP) {
        if(event.key.keysym.sym == SDLK_q) {
          /* Press q to quit the program */
          break; 
        } else if(event.key.keysym.sym == SDLK_f) {
          /* Press f to toggle fullscreen */
          if(!fullscreen) {
            /* Go Fullscreen */
            fullscreen = 1;
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
          } else {
            /* Leave Fullscreen */
            fullscreen = 0;
            SDL_SetWindowFullscreen(window, 0);
          }
        }
      }
    }

    draw(window, renderer);
  }

  /* Quit the program */
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

void draw(SDL_Window *window, SDL_Renderer *renderer) {
  const int fps = 60;
  const Uint32 ticksPerFrame = 1000 / fps; 
  SDL_DisplayMode mode;
  int w;
  int h;
  static Uint32 lastFrameTick = 0;
  Uint32 currentFrameTick = SDL_GetTicks();
  if(lastFrameTick == 0) {
    lastFrameTick = SDL_GetTicks();
  }

  Color bgColor = { .r = 0, .g = 0, .b = 0, .a = 255 }; 
  
  SDL_GetWindowDisplayMode(window, &mode);
  w = mode.w;
  h = mode.h;
  
  /* Clear the screen */
  SDL_SetRenderDrawColor(
      renderer,
      bgColor.r,
      bgColor.g,
      bgColor.b,
      bgColor.a
  );
  SDL_RenderClear(renderer);

  for(int i = 0; i < (100000); i++) {
    drawPixel(randomPixel(w, h), renderer);
  } 

  SDL_RenderPresent(renderer);

  if((currentFrameTick - lastFrameTick) >= ticksPerFrame) {
    lastFrameTick = currentFrameTick;
  } else {
    //printf("Delay needed\n");
    Uint32 elapsedTicks = currentFrameTick - lastFrameTick;
    SDL_Delay(ticksPerFrame - elapsedTicks);
  }
}

Pixel randomPixel(int w, int h) {
  Pixel pixel;
  
  pixel.point.x = rand() % w;
  pixel.point.y = rand() % h;
  pixel.color.r = rand() % 255;
  pixel.color.g = rand() % 255;
  pixel.color.b = rand() % 255;
  pixel.color.a = rand() % 255;

  return pixel;
}

void drawPixel(Pixel pixel, SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(
      renderer,
      pixel.color.r,
      pixel.color.g,
      pixel.color.b,
      pixel.color.a
  );

  SDL_RenderDrawPoint(renderer, pixel.point.x, pixel.point.y);
}
