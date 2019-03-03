#include <glm/glm.hpp>
#include "SDL2/SDL.h"
#include "glad/glad.h"

int main1()
{
  SDL_Init( SDL_INIT_VIDEO );
  SDL_CreateWindow("yo",0,0,640,480,SDL_WINDOW_OPENGL);
  SDL_Delay(1000);

  return 0;
}
