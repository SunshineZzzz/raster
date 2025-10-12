#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;
out vec3 color;
out vec2 uv;
uniform mat4 transform;
void main()
{
	// aPos作为attribute(属性)传入shader，不允许更改的
	vec4 position = vec4(aPos, 1.0);
	position = transform * position;
	gl_Position = position;
	color = aColor;
	uv = aUV;
}