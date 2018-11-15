#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCoord;

out vec3 outColor;

layout(std140) uniform LightSourceBlock
{
    vec3 colors[256];
};

void main() {
    gl_Position = vec4(aPos, 1.f);
    outColor = colors[1];
}