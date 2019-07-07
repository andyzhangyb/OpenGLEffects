#version 330 core
out vec4 FragColor;

in vec2 fragPos;

uniform samplerCube depthMap;

uniform vec2 lightPos;
uniform float farPlane;

void main() {
    float lightDistance = length(vec3(fragPos, 0.0) - vec3(lightPos.xy, 0.0)) / farPlane;
    float minDistance = texture(depthMap, vec3(fragPos - lightPos, 0.0)).r;
    // if (lightDistance > minDistance)
    //     FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    // else 
    //     FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    FragColor = vec4(minDistance, minDistance, minDistance, 1.0);
}
