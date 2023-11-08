#version 450 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;

uniform float time;

uniform int num;

uniform vec3 lightColor;
uniform vec3 lightPosition;

void main()
{
    // grey scale

    // float grey = (FragColor.r + FragColor.g + FragColor.b) / 3.0f;
    // FragColor = vec4(grey, grey, grey, 1.0f); 


    //flash light

    // float d = distance(TexCoord, vec2(0.5f, 0.5f));
    // d = exp(-d * 3.0f) * 2.0f;
    // if(d < 0.0f) {
    //     d = 0;
    // }
    // FragColor.rgb *= d;

    float ambientStrength = .3f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    float x = 10.0f * cos(time);
    float z = 10.0f * sin(time) + -3.0f;
    float y = 0.0f;
    // if(y < 0.0f)
    // {
    //     x = 0.0f;
    //     y = 0.0f;
    //     z = 0.0f;
    // }
    vec3 lightDir = normalize(vec3(x, y, z));

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    vec4 textureColor;
    if(num == 0) 
    {
        textureColor = texture(texture0, TexCoord);
    }
    else if(num == 1) 
    {
	    textureColor = texture(texture1, TexCoord);
    }
    else if(num == 2)
    {
	    textureColor = texture(texture2, TexCoord);
    }
    else if(num == 3)
    {
        textureColor = texture(texture3, TexCoord);
    }
    else if(num == 4)
    {
        textureColor = texture(texture4, TexCoord);
    }

    vec4 result = textureColor * vec4(ambient + diffuse, 1.0f);

    FragColor = result;
}
