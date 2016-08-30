#version 430 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;

//uniform mat4 MVP;

out vec3 pos;
out vec3 normal;

void main(){
    gl_Position.xyz = vPos;
    gl_Position.w = 1.0;
    pos = vPos;
    normal = vNorm;
}

