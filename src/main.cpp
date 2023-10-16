#include "../include/main.hpp"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>
const bool debugMode = 1;

//globabl variables
SDL_Window *gWindow = NULL;

SDL_Texture *gTexture = NULL;

SDL_Renderer *gRenderer = NULL;

SDL_Surface *gScreenSurface = NULL;

SDL_GLContext gContext;

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

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if(gRenderer == NULL)
    {
        printf("Renderer failed to be initialized! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    gContext = SDL_GL_GetCurrentContext();

    if(gContext == NULL)
    {
        printf("Context failed to be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        printf("Failed to initialize GLAD!\n");
        return false;
    }

    glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    // glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    int imgFlags = IMG_INIT_PNG;

    if(!(IMG_Init(imgFlags) & imgFlags)) 
    {
        printf("SDL Image could not be initialized! SDL Image Error: %s\n", IMG_GetError());
        return false;
    }

    gScreenSurface = SDL_GetWindowSurface(gWindow);
    
    return true;
}

bool loadMedia()
{
    bool success = true;

    gTexture = loadTexture("resources/roshar.png");
    if(gTexture == NULL)
    {
        printf("Failed to load PNG image!\n");
        success = false;
    }
    
    return success;
}

void close() 
{
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}

SDL_Texture *loadTexture(std::string path)
{
    SDL_Texture *texture = IMG_LoadTexture(gRenderer, path.c_str()); 
    if(texture == NULL)
    {
        printf("Unable to load image %s! SDL Image Error: %s\n", path.c_str(), IMG_GetError());
    }

    return texture;
}

void getVertices(SDL_Texture *texture, SDL_Rect *screen, SDL_Rect *sprite, float *vertices) 
{
    const float screen_width = 1920;
    const float screen_height = 1080;
    // printf("screen width: %f\n", screen_width);
    // printf("screen height: %f\n\n", screen_height);

    int image_width, image_height;
    SDL_QueryTexture(texture, NULL, NULL, &image_width, &image_height);
    // printf("image width: %d\n", image_width);
    // printf("image height: %d\n\n", image_height);

    float draw_left_corner_x = screen->x; 
    float draw_left_corner_y = screen->y;
    // printf("draw left corner at: (%f, %f)\n\n", draw_left_corner_x, draw_left_corner_y);

    float draw_width = screen->w;
    float draw_height = screen->h;
    // printf("draw width: %f, draw height: %f\n\n", draw_width, draw_height);

    float top_left_x, top_left_y;
    top_left_x = (draw_left_corner_x - (float)screen_width/2)/((float)screen_width/2);
    top_left_y = (draw_left_corner_y - (float)screen_height/2)/((float)screen_height/2) * -1;
    // printf("top left corner GL: (%f,%f)\n", top_left_x, top_left_y);

    float top_right_x, top_right_y;
    top_right_x = (draw_left_corner_x + draw_width - (float)screen_width/2)/((float)screen_width/2);
    top_right_y = (draw_left_corner_y - (float)screen_height/2)/((float)screen_height/2) * -1;
    // printf("top right corner GL: (%f,%f)\n", top_right_x, top_right_y);

    float bottom_left_x, bottom_left_y;
    bottom_left_x = (draw_left_corner_x - (float)screen_width/2)/((float)screen_width/2);
    bottom_left_y = (draw_left_corner_y + draw_height - (float)screen_height/2)/((float)screen_height/2) * -1;
    // printf("bottom left corner GL: (%f,%f)\n", bottom_left_x, bottom_left_y);

    float bottom_right_x, bottom_right_y;
    bottom_right_x = (draw_left_corner_x + draw_width - (float)screen_width/2)/((float)screen_width/2);
    bottom_right_y = (draw_left_corner_y + draw_height - (float)screen_height/2)/((float)screen_height/2) * -1;
    // printf("bottom right corner GL: (%f,%f)\n\n", bottom_right_x, bottom_right_y);

    draw_width = sprite->w;
    draw_height = sprite->h;

    float sprite_corner_x = sprite->x;
    float sprite_corner_y = sprite->y;
    // printf("sprite left corner at: (%f, %f)\n\n", sprite_corner_x, sprite_corner_y);

    float sprite_top_left_x, sprite_top_left_y;
    sprite_top_left_x = sprite_corner_x / image_width;
    sprite_top_left_y = sprite_corner_y / image_height;
    // printf("sprite top left corner GL at: (%f, %f)\n", sprite_top_left_x, sprite_top_left_y);
    
    float sprite_top_right_x, sprite_top_right_y;
    sprite_top_right_x = (sprite_corner_x + draw_width)/ image_width;
    sprite_top_right_y = sprite_corner_y / image_height;
    // printf("sprite top right corner GL at: (%f, %f)\n", sprite_top_right_x, sprite_top_right_y);
    
    float sprite_bottom_left_x, sprite_bottom_left_y;
    sprite_bottom_left_x = sprite_corner_x / image_width;
    sprite_bottom_left_y = (sprite_corner_y + draw_height) / image_height;
    // printf("sprite bottom left corner GL at: (%f, %f)\n", sprite_bottom_left_x, sprite_bottom_left_y);

    float sprite_bottom_right_x, sprite_bottom_right_y;
    sprite_bottom_right_x = (sprite_corner_x + draw_width)/ image_width;
    sprite_bottom_right_y = (sprite_corner_y + draw_height) / image_height;
    // printf("sprite bottom right corner GL at: (%f, %f)\n\n", sprite_bottom_right_x, sprite_bottom_right_y);
    
    float vertexArray[] = {
        top_left_x, top_left_y, 0.0f, sprite_top_left_x, sprite_top_left_y, //top-left
        top_right_x, top_right_y, 0.0f, sprite_top_right_x, sprite_top_right_y, //top-right
        bottom_left_x, bottom_left_y, 0.0f, sprite_bottom_left_x, sprite_bottom_left_y, //bottom-left
        bottom_left_x, bottom_left_y, 0.0f, sprite_bottom_left_x, sprite_bottom_left_y, //bottom-left
        top_right_x, top_right_y, 0.0f, sprite_top_right_x, sprite_top_right_y, //top-right
        bottom_right_x, bottom_right_y, 0.0f, sprite_bottom_right_x, sprite_bottom_right_y, //bottom-right
    };

    for(int i = 0; i < 30; i++){
        vertices[i] = vertexArray[i];
    }
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
            Shader ourShader("shaders/vertex.glsl","shaders/fragment.glsl");
            
            float *vertices = new float[30];

            // for(int i = 0; i < 30; i++)
            // {
            //     printf("%f, ", vertices[i]);
            //     if((i+1) % 5 == 0 && i != 1){
            //         printf("\n");
            //     }
            // }

            unsigned VAO;
            glGenVertexArrays(1, &VAO);

            glBindVertexArray(VAO);

            BatchRenderer batchRenderer = BatchRenderer();

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            
            bool quit = false;

            SDL_Event e;

            Uint64 last, curr;

            glm::mat4 mat = glm::mat4(1.0f);
            // mat = glm::translate(mat, glm::vec3(-0.0f, 0.0f, 0.0f));

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
                            case SDLK_UP:
                                mat = glm::translate(mat, glm::vec3(0.1f, 0.0f, 0.0f));
                                break;
                            case SDLK_DOWN:
                                mat = glm::translate(mat, glm::vec3(-0.1f, 0.0f, 0.0f));
                                break;
                        }
                    }
                }

                glClearColor(0x00, 0x00, 0x00, 0xFF);
                glClear(GL_COLOR_BUFFER_BIT);

                ourShader.use();
                // unsigned int loc = glGetUniformLocation(ourShader.ID, "t");
                 
                // glUniform1i(loc, 1);
                glActiveTexture(GL_TEXTURE0);
                SDL_GL_BindTexture(gTexture, NULL, NULL);
               
                int w,h;
                SDL_QueryTexture(gTexture, NULL, NULL, &w, &h);

                float x,y;

                SDL_Rect sc = {0, 0, w, h};

                SDL_Rect sp = {0, 0, w, h};

                // for(int i = 0; i < 100; i++)
                // {
                //     x = (float)std::rand() / RAND_MAX * SCREEN_WIDTH;
                //     y = (float)std::rand() / RAND_MAX * SCREEN_HEIGHT;
                //     sc.x = x;
                //     sc.y = y;
                //     getVertices(gTexture, &sc, &sp, vertices);
                //     batchRenderer.addVertices(vertices);
                //     batchRenderer.render(); 
                // }
                unsigned int transform = glGetUniformLocation(ourShader.ID, "transform");
                glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(mat));
                getVertices(gTexture, &sc, &sp, vertices);
                batchRenderer.addVertices(vertices);
                batchRenderer.render(); 
                
                // glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                // glUniform1i(loc,0);
                // glDrawArrays(GL_TRIANGLES, 0, 6);

                SDL_GL_SwapWindow(gWindow);
                curr = SDL_GetTicks64();
                // printf("frames per second: %f\n", 1.0f / ((float)(curr - last) / 1000));
            }
        }
    }

    close();

    return 0;
}

