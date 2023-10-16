#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <vector>

#include "shader.hpp"
#include "batch_renderer.hpp"
//functions
bool init();

bool loadMedia();

void close();

SDL_Texture *loadTexture(std::string path);

void getVertices(SDL_Texture *texture, SDL_Rect *screen, SDL_Rect *sprite, float *vertices);

//constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;


