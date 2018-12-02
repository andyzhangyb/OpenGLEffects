#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 position;
out vec2 texPosition;

void main() {
    position = aPos.xy;
    texPosition = aTexCoord;
    gl_Position = vec4(aPos, 1.f);
}