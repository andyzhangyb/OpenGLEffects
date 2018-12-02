#version 330 core
out vec4 FragColor;

in vec2 position;
in vec2 texPosition;

layout(std140) uniform LightSourceBlock
{
    float waveSpeed;
    float currentTime;
    vec3 points[256];
};

void main() {
    float waveHight = 0;
    float Frequency = 0.5;
    for (int i = 0; i < 256; i++) {
        vec3 point = points[i];
        if (point.x == 0.f && point.y == 0.f && point.z == 0.f)
            break;
        float timeDiff = currentTime - point.z;
        // float distanceOfClick = distance(position, point.xy);
        float distanceOfClick = distance(position, vec2(0, 0));
        float waveRadius = timeDiff * waveSpeed;

        float decayOfTime = timeDiff + 1;
        float decayOfDistance = (waveRadius - distanceOfClick) * 0.8;
        float v = (sin(Frequency * (waveRadius - distanceOfClick) * 100 / 3.14 * 4) + 1);
        waveHight = waveHight + v * 0.5;
            break;
    }
    FragColor = vec4(texPosition, waveHight, 1.f);
}