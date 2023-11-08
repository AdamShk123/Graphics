#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform mat4 rotate;

void main()
{
    vec4 rotate = vec4(aPos, 1.0f) * rotate;
    Normal = cross(aPos, rotate.xyz);
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
    FragPos = vec3(model * vec4(aPos, 1.0f));
    TexCoord = aTexCoord;
}
