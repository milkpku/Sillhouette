#version 430 core

in vec3 Position_worldspace;
in vec3 Normal_worldspace;
//in vec3 EyeDirection_worldspace;
in vec3 CameralDirection_worldspace;

//uniform Light{
//    vec4 lightColor;
//    vec3 lightPos;
//    vec4 globalAmb;
//}

out vec4 color;

void main()
{
    vec3 l = normalize(CameralDirection_worldspace);
    vec3 n = normalize(Normal_worldspace);
    float cos = clamp(dot(n, l), 0, 1);

    vec3 amb = vec3(0.1, 0.1, 0.1);
    vec3 light = vec3(0.8, 0.8, 0.8);

    vec3 sight = amb + light * cos;
    color = vec4(sight, 0.8);
}
