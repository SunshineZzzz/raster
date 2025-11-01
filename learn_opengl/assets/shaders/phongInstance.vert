#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;
out vec2 uv;
out vec3 normal;
out vec3 worldPosition;
// 还是保留，可以整体调整
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform mat4 matrices[100];
void main()
{
	// 将输入的顶点位置，转化为齐次坐标(3维-4维)
	vec4 transformPosition = vec4(aPos, 1.0);
	// 做一个中间变量TransformPosition，用于计算四位位置与modelMatrix相乘的中间结果
	transformPosition = modelMatrix * matrices[gl_InstanceID] * transformPosition;
	// 计算当前顶点的worldPosition，并且向后传输给FragmentShader
	worldPosition = transformPosition.xyz;
	gl_Position = projectionMatrix * viewMatrix * transformPosition;
	uv = aUV;
	normal = normalMatrix * aNormal;
}