#version 430 core

in vec3 pos;
in vec3 normal;

//uniform Position{
//   vec3 eyePos;
//}

//uniform Light{
//    vec4 lightColor;
//    vec3 lightPos;
//    vec4 globalAmb;
//}

//uniform mat4 MVP;

out vec4 color;

void main()
{
    vec3 E = vec3(0,0,1) - pos;
    float cos = dot(normalize(normal), normalize(E));

    vec3 amb = vec3(0.1, 0.1, 0.1);
    vec3 light = vec3(0.8, 0.8, 0.8);

    vec3 sight = amb + light * cos;
    color = vec4(sight, 0.8);
}
