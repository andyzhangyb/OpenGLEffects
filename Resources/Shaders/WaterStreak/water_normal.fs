#version 330 core
out vec4 FragColor;

in vec2 position;
in vec2 texPosition;

layout(std140) uniform ClickPointsBlock
{
    float currentTime;
    vec3 points[256];
};

void main() {
    float waveHight = 0;
    float frequency = 80;
    float speed = 3.14 / 10;

    float dbDistance = 0.04;
    float dbOut = 0.3;
    float dbIn = 0.1;

    for (int i = 0; i < 256; i++) {
        vec3 point = points[i];
        if (point.x == 0.f && point.y == 0.f && point.z == 0.f)
            break;
        float timeDiff = currentTime - point.z;
        float distanceOfClick = distance(position, point.xy);
        float waveRadius = timeDiff * speed;

        float v = cos(frequency * (distanceOfClick - waveRadius)) / 50 + 1 / 50;
        if (v < 0)
            v = -v;
        if (distanceOfClick > waveRadius) { // 圈外
            v = v - (distanceOfClick - waveRadius) * dbOut - dbDistance * waveRadius;
        } else { //圈内
            v = v - (waveRadius - distanceOfClick) * dbIn - dbDistance * waveRadius;
        }
        if (v < 1 / 50)
            v = 1 / 50;

        waveHight += v;
    }
    FragColor = vec4(texPosition.xy, waveHight, 1.f);
}