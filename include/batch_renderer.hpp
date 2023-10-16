#include <glad/glad.h>
#include <stdio.h>

class BatchRenderer 
{
private:
    float *vertices;
    int count = 0;
    unsigned int VBO;
public:
    BatchRenderer();

    void addVertices(float *vertices);
    void render();
};
