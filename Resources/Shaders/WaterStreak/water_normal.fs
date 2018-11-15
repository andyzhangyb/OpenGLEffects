#version 330 core
out vec4 FragColor;

in vec3 outColor;

void main() {
    // FragColor = vec4(1.f, 0.f, 0.f, 1.f);
    FragColor = vec4(outColor, 1.f);
}