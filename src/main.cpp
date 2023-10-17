#include "../include/main.hpp"

//globabl variables
SDL_Window *gWindow = NULL;

SDL_Texture *gTexture = NULL;

SDL_Renderer *gRenderer = NULL;

bool init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    
    if(gWindow == NULL)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    if(gRenderer == NULL){
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("IMG could not be initialized! IMG Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void close() 
{
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

bool loadMedia() 
{
    gTexture = IMG_LoadTexture(gRenderer, "resources/samus.png");
    if(gTexture == NULL)
    {
        printf("failed to load image! IMG Error: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

int main(int argc, char *args[])
{
    if(!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        if(!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            bool quit = false;

            SDL_Event e;

            Uint64 last, curr;

            while(!quit)
            {
                last = SDL_GetTicks64();
                while(SDL_PollEvent(&e) != 0)
                {
                    if(e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    else if(e.type == SDL_KEYDOWN)
                    {
                        switch (e.key.keysym.sym) 
                        {
                            case SDLK_ESCAPE:
                                quit = true;
                                break;
                        }
                    }
                }

                SDL_SetRenderDrawColor(gRenderer, 0xF0, 0xF0, 0xF0, 0xFF);
                SDL_RenderClear(gRenderer);

                int w,h;
                SDL_QueryTexture(gTexture, NULL, NULL, &w, &h);

                SDL_Rect src = {0, 0, w, h};
                SDL_Rect dst = {0, 0, w, h};

                SDL_RenderCopy(gRenderer, gTexture, &src, &dst);

                SDL_RenderPresent(gRenderer);

                curr = SDL_GetTicks64();
                // printf("frames per second: %f\n", 1.0f / ((float)(curr - last) / 1000));
            }
        }
    }

    close();

    return 0;
}

