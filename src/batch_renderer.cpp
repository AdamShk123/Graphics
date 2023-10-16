#include "../include/batch_renderer.hpp"

BatchRenderer::BatchRenderer() {
    this->vertices = new float[3000];
    glGenBuffers(1, &this->VBO); 
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
}

void BatchRenderer::addVertices(float *vertices)
{
    for(int i = 0; i < 30; i++){
        this->vertices[this->count * 5 + i] = vertices[i];
    }
    this->count += 6;
    if(this->count * 5 == 3000){
        // printf("called render!");
        this->render();
    }
}

void BatchRenderer::render()
{
    glBufferData(GL_ARRAY_BUFFER, this->count * 5 * sizeof(float), vertices, GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, this->count);

    this->count = 0;
}
