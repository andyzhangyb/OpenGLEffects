#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D textureBg;
uniform sampler2D textureNormal;

void main() {
    vec3 centerPoint = texture(textureNormal, texCoord).xyz;
    vec3 right = texture(textureNormal, texCoord + vec2(1.f / 1000.0, 0)).xyz;
    vec3 up = texture(textureNormal, texCoord + vec2(0, 1.f / 1000.0)).xyz;
    
	vec3 normal = normalize(cross(right - centerPoint, up - centerPoint));
	vec3 I = normalize(vec3(0, 0, 1) - vec3(texCoord, -1));
	vec3 R = refract(I, normal, 1.f / 1.333);

    FragColor = texture(textureBg, R.xy);
}