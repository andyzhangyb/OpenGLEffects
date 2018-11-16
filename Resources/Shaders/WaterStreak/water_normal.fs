#version 330 core
out vec4 FragColor;

in vec3 outwaveHight;

void main() {
    FragColor = vec4(outwaveHight, 1.f);
}