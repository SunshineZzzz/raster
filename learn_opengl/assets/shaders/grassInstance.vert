#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aColor;
layout (location = 4) in mat4 aInstanceMatrix;
out vec2 uv;
out vec3 normal;
out vec3 worldPosition;
out vec2 worldXZ;
// 还是保留，可以整体调整
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

// 风力相关
// 时间
uniform float time;
// 风的强度
uniform float windScale;
// 风的方向
uniform vec3 windDirection;
// 相位调节
uniform float phaseScale;

void main()
{
	// 将输入的顶点位置，转化为齐次坐标(3维-4维)
	vec4 transformPosition = vec4(aPos, 1.0);
	// 做一个中间变量TransformPosition，用于计算四位位置与modelMatrix相乘的中间结果
	transformPosition = modelMatrix * aInstanceMatrix * transformPosition;
	// 下面要做风摇摆，但是风摇摆需要用到x和z，所以先保存起来
	worldXZ = transformPosition.xz;
	// 风力变动
	// 风力方向
	vec3 windDirN = normalize(windDirection);
	float phaseDistance = dot(windDirN, transformPosition.xyz);
	// (1.0 - aColor.r) - 顶点颜色和风力结合在一起，最顶点r=0,g=0,b=0，其他顶点r=1,g=0,b=0，
	// 插值以后中间像素1~0，风力假设是10，插值出中间风力10~0。需要反转一下1.0 - aColor.r
	// 从而做到下面不动，上面动
	// (xxx) * windScale 前者是一个标量，后者是一个向量。还是下面不动，上面动。
	// time + phaseDistance，顶点在风上的投影作为相位，从而让风的方向和顶点位置有关
	transformPosition += vec4((sin(time + phaseDistance/ phaseScale) * (1.0 - aColor.r) * windScale) * windDirN, 0.0);
	// 计算当前顶点的worldPosition，并且向后传输给FragmentShader
	worldPosition = transformPosition.xyz;
	gl_Position = projectionMatrix * viewMatrix * transformPosition;
	uv = aUV;
	// 实例化绘制没法用c++计算并且传递了
	// normal = normalMatrix * aNormal;
	normal = transpose(inverse(mat3( modelMatrix * aInstanceMatrix ))) * aNormal;
}