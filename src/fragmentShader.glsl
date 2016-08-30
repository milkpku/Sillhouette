#version 430 core

in vec3 pos;
in vec3 normal;

uniform Position{
    vec3 eyePos;
}

uniform Light{
    vec4 lightColor;
    vec3 lightPos;
    vec4 globalAmb;
}

uniform MVP{
    mat4 MVP;
}

out vec4 color;

void main()
{
    vec3 E = eyePos - pos;
    float cos_silhouette = dot(normalize(normal), normalize(E));

    if (abs(cos_silhouette) < 1e-3)
    {
        color = vec4(1.0, 0.0, 0.0, 0.5);
    }
}
