#version 330 core
in vec3 FragPos;

uniform vec2 lightPos;
uniform float farPlane;

void main() {
    float lightDistance = length(FragPos - vec3(lightPos, 0.0));
    lightDistance = lightDistance / farPlane;
    gl_FragDepth = lightDistance;
}