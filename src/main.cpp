#include "../include/main.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>

SDL_Window *gWindow = NULL;

SDL_GLContext gContext;

Texture texture0;
Texture texture1;
Texture texture2;
Texture texture3;
Texture texture4;

bool init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    
    if(gWindow == NULL)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    gContext = SDL_GL_CreateContext(gWindow);
    if(!gContext){
        printf("Failed to create openGL context!");
        return false;
    }

    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) 
    {
        printf("Failed to initialize glad!\n");
        return false;
    }

    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

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
    if(!loadTexture("resources/tiles.png", &texture0, false))
    {
        printf("Failed to load texture 0!\n");
        return false;
    }

    if(!loadTexture("resources/rocks.png", &texture1, false))
    {
        printf("Failed to load texture 1!\n");
        return false;
    }

    if(!loadTexture("resources/trees.png", &texture2, false))
    {
        printf("Failed to load texture 2!\n");
        return false;
    }

    if(!loadTexture("resources/awesomeface.png", &texture3, false))
    {
        printf("Failed to load texture 3!\n");
        return false;
    }

    if(!loadTexture("resources/sky.png", &texture4, false))
    {
        printf("Failed to load texture 4!\n");
        return false;
    }

    return true;
}

bool loadTexture(std::string path, Texture *texture, bool rgb)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;

    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if(data)
    {
        if(rgb)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else 
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
        printf("Failed to load texture!\n");
        return false;
    }
    
    stbi_image_free(data);

    texture->id = id;
    texture->w = width;
    texture->h = height;

    return true;
}

void getVertices(Texture *texture, Rect<float> *src, Rect<float> *dest, std::vector<Vertex> *vertices, float z) 
{
    float a, b, c, d;
    a = (dest->x - (float)SCREEN_WIDTH/2) / ((float)SCREEN_WIDTH/2) * 16.0f / 9.0f;
    b = (dest->y - (float)SCREEN_HEIGHT/2) / ((float)SCREEN_HEIGHT/2) * -1;
    c = (dest->x + dest->w - (float)SCREEN_WIDTH/2) / ((float)SCREEN_WIDTH/2) * 16.0f / 9.0f;
    d = (dest->y + dest->h - (float)SCREEN_HEIGHT/2) / ((float)SCREEN_HEIGHT/2) * -1;

    float e, f, g, h;
    e = (src->x) / ((float)texture->w);
    f = 1.0f - (src->y) / ((float)texture->h);
    g = (src->x + src->w) / ((float)texture->w);
    h = 1.0f - (src->y + src->h) / ((float)texture->h);

    Vertex v1 = {a, b, z, e, f};
    Vertex v2 = {c, b, z, g, f};
    Vertex v3 = {a, d, z, e, h};
    Vertex v4 = {c, d, z, g, h};
   
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v3);
    vertices->push_back(v2);
    vertices->push_back(v3);
    vertices->push_back(v4);
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

            std::vector<Vertex> vertices = std::vector<Vertex>();

            unsigned int VBO, VAO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);
            
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 1000, nullptr, GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture0.id);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture1.id);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, texture2.id);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, texture3.id);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, texture4.id);

            ourShader.use();
            ourShader.setInt("texture0", 0);
            ourShader.setInt("texture1", 1);
            ourShader.setInt("texture2", 2);
            ourShader.setInt("texture3", 3);
            ourShader.setInt("texture4", 4);

            bool quit = false;

            SDL_Event e;

            Uint64 start, end;

            float transX = 0.0f;
            float transY = 0.0f;

            float zoom = 0.0f;

            glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
            glm::vec3 lightPosition = glm::vec3(0.0f, 0.0f, -2.0f);

            ourShader.setVec3("lightColor", lightColor);
            ourShader.setVec3("lightPosition", lightPosition);

            glm::mat4 rotate = glm::mat4(1.0f);
            rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));

            ourShader.setMat4("rotate", rotate);

            float time = 0.0f;

            while(!quit)
            {
                start = SDL_GetTicks64();
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
                            case SDLK_d:
                                transX -= 0.1f;
                                break;
                            case SDLK_a:
                                transX += 0.1f;
                                break;
                            case SDLK_w:
                                transY -= 0.1f;
                                break;
                            case SDLK_s:
                                transY += 0.1f;
                                break;
                            case SDLK_UP:
                                zoom += 0.1f;
                                break;
                            case SDLK_DOWN:
                                zoom -= 0.1f;
                                break;
                             case SDLK_LEFT:
                                time += 0.1f;
                                break;
                            case SDLK_RIGHT:
                                time -= 0.1f;
                                break;
                        }
                    }
                }
                
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

                // printf("%f\n", start / 1000.0f);
                // ourShader.setFloat("time", start / 1000.0f);
                ourShader.setFloat("time", time);
                printf("x: %f, y: 0.0f, z: %f\n", 100.0f * std::cos(time), 100.0f * std::sin(time) - 3.0f);

                glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                // glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
                glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                Rect<float> src4 = {0, 0, 16, 16};
                Rect<float> dest4 = {16 * 4 * 5, SCREEN_HEIGHT - 16 * 16, 16 * 4, 16 * 4};

                Rect<float> src5 = {16, 0, 16, 16};
                Rect<float> dest5 = {16 * 4 * 6, SCREEN_HEIGHT - 16 * 16, 16 * 4, 16 * 4};

                Rect<float> src6 = {16 * 2, 0, 16, 16};
                Rect<float> dest6 = {16 * 4 * 7, SCREEN_HEIGHT - 16 * 16, 16 * 4, 16 * 4};

                Rect<float> src7 = {16 * 3, 0, 16, 16};
                Rect<float> dest7 = {16 * 4 * 8, SCREEN_HEIGHT - 16 * 16, 16 * 4, 16 * 4};

                Rect<float> src8 = {16 * 4, 0, 15, 16};
                Rect<float> dest8 = {16 * 4 * 9, SCREEN_HEIGHT - 16 * 16, 16 * 4, 16 * 4};

                Rect<float> src9 = {0, 1, 37.0f, 150.0f};
                Rect<float> dest9 = {-40.0f * 8 * 4, 0, 40.0f * 8, 150.0f * 8};

                Rect<float> src10 = {0, 150.0f, 40.0f, 135.0f};
                Rect<float> dest10 = {-40.0f * 8 * 3, 80.0f, 40.0f * 8, 135.0f * 8};
 
                Rect<float> src11 = {0, 0, 60.0f, 80.0f};
                Rect<float> dest11 = {0, 60.0f * 8, 60.0f * 8, 80.0f * 8};

                Rect<float> src12 = {0, 81.0f, 60.0f, 80.0f};
                Rect<float> dest12 = {60.0 * 8, 60.0f * 8 * 1.6f, 60.0f * 8, 80.0f * 8};

                Rect<float> src13 = {0, 0, (float)texture4.w, (float)texture4.h};
                Rect<float> dest13 = {-1000.0f, -1000.0f, (float)texture4.w * 12, (float)texture4.h * 12};

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(transX, transY, 0.0f));
                model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

                glm::mat4 view = glm::mat4(1.0f);
                view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f + zoom));
           
                glm::mat4 proj = glm::perspective<float>(glm::radians(45.0f), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f);
                // float aspect = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
                // glm::mat4 proj = glm::ortho(-aspect, aspect, -1.0f, 1.0f, 0.1f, 100.0f);

                ourShader.setMat4("model", model);
                ourShader.setMat4("view", view);
                ourShader.setMat4("proj", proj);


                //background
                getVertices(&texture4, &src13, &dest13, &vertices, -3.0f);
                ourShader.setInt("num", 4);
                glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
                glDrawArrays(GL_TRIANGLES, 0, vertices.size());
                vertices.clear();
                
                //trees
                for(int i = 0; i < 8; i++)
                {
                    getVertices(&texture3, &src9, &dest9, &vertices, -2.0f);
                    getVertices(&texture3, &src10, &dest10, &vertices, -2.0f);
                    dest9.x += 40.0f * 8 * 2;
                    dest10.x += 40.0f * 8 * 2;
                }

                ourShader.setInt("num", 2);
                glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
                glDrawArrays(GL_TRIANGLES, 0, vertices.size());
                vertices.clear();

                //rocks
                for(int i = 0; i < 5; i++)
                {
                    getVertices(&texture1, &src11, &dest11, &vertices, -1.0f);
                    getVertices(&texture1, &src12, &dest12, &vertices, -1.0f);
                    dest11.x += 60.0f * 8 * 2;
                    dest12.x += 60.0f * 8 * 2;
                }

                ourShader.setInt("num", 1);
                glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
                glDrawArrays(GL_TRIANGLES, 0, vertices.size());
                vertices.clear();

                //tiles
                for(int i = 0; i < 5; i++)
                {
                    getVertices(&texture0, &src4, &dest4, &vertices, 0.0f);

                    for(int x = 0; x < 10; x++)
                    {
                        getVertices(&texture0, &src5, &dest5, &vertices, 0.0f);
                        getVertices(&texture0, &src6, &dest6, &vertices, 0.0f);
                        getVertices(&texture0, &src7, &dest7, &vertices, 0.0f);

                        dest5.x += 16 * 4 * 3;
                        dest6.x += 16 * 4 * 3;
                        dest7.x += 16 * 4 * 3;
                    }

                    dest5.x -= 16 * 4 * 30;
                    dest6.x -= 16 * 4 * 30;
                    dest7.x -= 16 * 4 * 30;
                   
                    dest8.x += 16 * 4 * 27;

                    getVertices(&texture0, &src8, &dest8, &vertices, 0.0f);

                    dest8.x -= 16 * 4 * 27;

                    src4.y += 16;
                    dest4.y += 16 * 4;
                    src5.y += 16;
                    dest5.y += 16 * 4;
                    src6.y += 16;
                    dest6.y += 16 * 4;
                    src7.y += 16;
                    dest7.y += 16 * 4;
                    src8.y += 16;
                    dest8.y += 16 * 4;
                }

                ourShader.setInt("num", 0);
                glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
                glDrawArrays(GL_TRIANGLES, 0, vertices.size());
                vertices.clear();

                //foreground
                
                SDL_GL_SwapWindow(gWindow);

                end = SDL_GetTicks64();
                if((float)(end - start) / 1000.0f > 60.0f){
                    SDL_Delay((float)(end - start)/1000.0f - 60.0f);
                }
                // printf("frames per second: %f\n", 1.0f / ((float)(end - start) / 1000.0f));
            }
        }
    }

    close();

    return 0;
}

