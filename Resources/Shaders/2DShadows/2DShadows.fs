#version 330 core
out vec4 FragColor;

in vec3 fragPos;

uniform samplerCube depthMap;

uniform vec2 lightPos;
uniform float farPlane;

uniform int drawTrueColor;

void main() {
    float lightDistance = length(fragPos - vec3(lightPos.xy, 0.0)) / farPlane;
    float minDistance = texture(depthMap, fragPos - vec3(lightPos.xy, 0.0)).r;
    if (drawTrueColor == 1)
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else
        if (lightDistance > minDistance)
            FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        else 
            FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    // FragColor = vec4(minDistance, minDistance, minDistance, 1.0);
}
