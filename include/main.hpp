#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
#include <SDL2/SDL_blendmode.h>

#include <SDL2/SDL_mixer.h>

#include "./shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"

#include <vector>
#include <string>

template<class T>
struct Rect 
{
    T x;
    T y;
    T w;
    T h;
};

struct Texture 
{
    unsigned int id;
    unsigned int w;
    unsigned int h;
};

struct Vertex 
{
    float pos[3];
    float tex[2];
};

//functions
bool init();

void close();

bool loadMedia();

bool loadTexture(std::string path, Texture *texture, bool rgb);

void getVertices(Texture *texture, Rect<float> *src, Rect<float> *dest, std::vector<Vertex> *vertices, float z);

//constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;


