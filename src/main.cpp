#include "../include/main.hpp"

//globabl variables
SDL_Window *gWindow = NULL;

SDL_Texture *gTexture = NULL;

SDL_Renderer *gRenderer = NULL;

SDL_Surface *gScreenSurface = NULL;

SDL_Texture *gHelloWorld = NULL;

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

    gHelloWorld = loadTexture("resources/roshar.png");
    if(gHelloWorld == NULL)
    {
        printf("Failed to load PNG image!\n");
        success = false;
    }
    
    return success;
}

void close() 
{
    SDL_DestroyTexture(gHelloWorld);
    gHelloWorld = NULL;

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

            float vertices[] = {
                -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, // top-left
                 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, // top-right
                -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // bottom-left
                 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, // bottom-right
                 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, // top-right
            }; 

            unsigned VAO, VBO, EBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            // glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            
            bool quit = false;

            SDL_Event e;

            while(!quit)
            {
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

                glClearColor(0x00, 0x00, 0x00, 0xFF);
                glClear(GL_COLOR_BUFFER_BIT);

                ourShader.use();
                unsigned int loc = glGetUniformLocation(ourShader.ID, "t");
                 
                glUniform1i(loc, 1);
                glBindVertexArray(VAO);
                glActiveTexture(GL_TEXTURE0);
                SDL_GL_BindTexture(gHelloWorld, NULL, NULL);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                 
                // glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                glUniform1i(loc,0);
                glDrawArrays(GL_TRIANGLES, 0, 6);

                SDL_GL_SwapWindow(gWindow);
            }
        }
    }

    close();

    return 0;
}

