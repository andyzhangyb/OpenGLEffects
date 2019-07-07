#version 330 core
in vec4 FragPos;

uniform vec2 lightPos;
uniform float farPlane;

void main() {
    float lightDistance = length(vec3(FragPos.xy, 0.0) - vec3(lightPos, 0.0));
    lightDistance = lightDistance / farPlane;
    gl_FragDepth = lightDistance;
}