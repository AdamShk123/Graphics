#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0f);
    ourColor = vec3(1.0, 0.0f, 0.0f);

    //flip tex coord vertically 0.5 - (y - 0.5) 
    //flip tex coord horizontally 0.5 - (x - 0.5) 

    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
