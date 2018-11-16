#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D textureBg;
uniform sampler2D textureNormal;

void main() {
    // FragColor = texture(textureBg, texCoord);

    vec3 centerPoint = texture(textureNormal, texCoord).xyz;
    vec3 point1 = texture(textureNormal, texCoord + vec2(1.f / 800, 1.f / 600)).xyz;
    vec3 point2 = texture(textureNormal, texCoord + vec2(1.f / 600, 1.f / 800)).xyz;
    
	vec3 v_in = normalize(vec3(0, 0, 1) - vec3(0, 0, -1));
	vec3 normal = normalize(cross(point1 - centerPoint, point2 - centerPoint));
	vec3 v_out = refract(v_in, normal, 1.333);

    FragColor = vec4(normal, 1.f);
    FragColor = texture(textureBg, v_out.xy);
    // FragColor = texture(textureNormal, texCoord);
}