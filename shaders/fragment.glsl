#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;

uniform bool t;

void main()
{
    FragColor = texture(texture1, TexCoord);
    // if(t)
    // {
    //     FragColor = texture(texture1, TexCoord);
    //     // vec4 c = texture(texture1, TexCoord);
    //     // float greyValue = (c.x + c.y + c.z) / 3;
    //     // FragColor = vec4(greyValue, greyValue, greyValue, 1.0f);
    //     // float r = c.x * 1.4;
    //     // float g = c.y * 0.9;
    //     // float b = c.z * 0.9;
    //     // FragColor = vec4(r, g, b, 1.0f);
    // }
    // else
    // {
    //     FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    // }
}
