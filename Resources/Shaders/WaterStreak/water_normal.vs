#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCoord;

out vec3 outwaveHight;

layout(std140) uniform LightSourceBlock
{
    float waveSpeed;
    float currentTime;
    vec3 points[256];
};

void main() {
    gl_Position = vec4(aPos, 1.f);
    float waveHight = 0;
    float Frequency = 1;
    for (int i = 0; i < 256; i++) {
        vec3 point = points[i];
        if (point.x == 0.f && point.y == 0.f)
            break;
        float timeDiff = currentTime - point.z;
        float distanceOfClick = distance(aPos.xy, point.xy);
        float waveRadius = timeDiff * waveSpeed;

        float decayOfTime = timeDiff + 1;
        float decayOfDistance = (waveRadius - distanceOfClick) * 0.8;
        float v = (sin(Frequency * (waveRadius - distanceOfClick) * 100 / 3.14 * 4));
        waveHight = waveHight + v * 0.5;
    }
    // outwaveHight = vec3(waveHight, waveHight, waveHight);
    outwaveHight = vec3(aPos.xy, waveHight / 10);
}