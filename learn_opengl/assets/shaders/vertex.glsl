#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;
out vec2 uv;
out vec3 normal;
out vec3 worldPosition;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
void main()
{
	vec4 transformPosition = vec4(aPos, 1.0);
	// 经过线性变化+平移后，顶点所在的坐标，这个是omega肯定是1
	transformPosition = modelMatrix * transformPosition;
	worldPosition = transformPosition.xyz;
	gl_Position = projectionMatrix * viewMatrix * transformPosition;
	uv = aUV;
	// transpose(inverse(mat3(modelMatrix))) 就是法线矩阵
	// 建议在CPU端计算法线矩阵，传进来就行
	normal = transpose(inverse(mat3(modelMatrix))) * aNormal;
}