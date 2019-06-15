#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D textureBg;
uniform sampler2D textureNormal;

void main() {
    vec3 centerPoint = texture(textureNormal, texCoord).xyz;
    vec3 left = texture(textureNormal, texCoord - vec2(1.f / 100.0, 0)).xyz;
    vec3 right = texture(textureNormal, texCoord + vec2(1.f / 100.0, 0)).xyz;
    vec3 up = texture(textureNormal, texCoord + vec2(0, 1.f / 100.0)).xyz;
    vec3 down = texture(textureNormal, texCoord - vec2(0, 1.f / 100.0)).xyz;
    
	vec3 normal = normalize(cross(left - right, down - up));
	vec3 R = refract(vec3(0, 0, -1), normal, 1.333);

    FragColor = texture(textureBg, texCoord.xy + R.xy);
}