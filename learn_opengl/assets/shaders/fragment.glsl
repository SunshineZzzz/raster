#version 460 core
out vec4 FragColor;
in vec2 uv;
in vec3 normal;
uniform sampler2D sampler;
void main()
{
	vec3 normalN = normalize(normal);
	vec3 normalColor = clamp(normalN, 0.0, 1.0);
	FragColor = vec4(normalColor, 1.0);
}