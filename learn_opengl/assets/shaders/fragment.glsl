#version 460 core
out vec4 FragColor;
in vec3 color;
in vec2 uv;
uniform sampler2D sampler;
void main()
{
	// FragColor = vec4(color, 1.0);
	FragColor = texture(sampler, uv);
}