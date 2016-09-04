#include <SDL2/SDL.h>

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265

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

void drawSineWave(int w, int h, SDL_Renderer *renderer);
void drawGrid(int w, int h, SDL_Renderer *renderer);

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
   
  //drawSineWave(w, h, renderer); 
  drawGrid(w, h, renderer);

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

void drawGrid(int w, int h, SDL_Renderer *renderer) {
  int cols = 30;
  int rows = 30;
  int cellWidth = roundf((float)w / (float)cols);
  int cellHeight = roundf((float)h / (float)rows);
  Color gridColor = {0, 255, 0, 255};
  Pixel gridPixels[w * (cols - 1) + h * (rows - 1)]; 
  int index = 0;

  static float startAngle = PI;
  static int amp = -1;
  static float frequency = (PI / 120);
  float angle = startAngle;
  int maxAmp = cellHeight / 5;
  int ampStep = cellHeight / 20;
  
  amp = maxAmp;

  /* Draw Columns */
  for(int i = 1; i < cols; i++) {
    int x = i * cellWidth;
    for(int j = 0; j < h; j++) {
      Point p = {sin(angle) * amp + x, j};
      Pixel pix = {p, gridColor};
      gridPixels[index] = pix;
      index++;
      drawPixel(pix, renderer);
      angle += frequency;
    }
  }

  /* Draw Rows */
  for(int i = 1; i < rows; i++) {
    int y = i * cellHeight;
    for(int j = 0; j < w; j++) {
      Point p = {j, sin(angle) * amp + y};
      Pixel pix = {p, gridColor};
      gridPixels[index] = pix;
      index++;
      drawPixel(pix, renderer);
      angle += frequency;
    }
  }

  startAngle += frequency;
  /*
  amp += ampStep * ((rand() % 2) > 0 ? -1 : 1);
  if(abs(amp) > maxAmp) {
    if(amp < 0) {
      amp = maxAmp * -1;
    } else {
      amp = maxAmp;
    }
  }
  */
  
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

void drawSineWave(int w, int h, SDL_Renderer *renderer) {
  static float startAngle = PI;
  static int amp = -1;
  static float frequency = (PI / 60);

  Color c = {0, 255, 0, 255};
  float angle = startAngle;
  
  for(int i = 0; i < w; i++) {
    Point p = {i, sin(angle) * amp + (h / 2)};
    Pixel pix = {p, c};
    drawPixel(pix, renderer);
    angle += frequency;
  }

  startAngle += frequency;
  amp += (h / 120) * ((rand() % 2) > 0 ? -1 : 1);
}
