#version 460 core
layout (location = 0) in vec3 aPos;
// 直接用包围盒的aPos作为UVW，1*1*1的包围盒
// layout (location = 1) in vec2 aUV;
out vec3 uvw;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
void main()
{
	vec4 transformPosition = vec4(aPos, 1.0);
	transformPosition = modelMatrix * transformPosition;
	// mvp后，剪裁坐标系下的顶点坐标
	gl_Position = projectionMatrix * viewMatrix * transformPosition;
	// 核心修改：深度欺骗 (Depth Hack)
	// 后面需要经历透视除法，这样子修改后，透视除法一直都是远平面，深度值一直是1！
	// 天空盒子是1X1X1,near是2.0f far是1000.0f,mvp就会被裁剪，这样做避免了剪裁，永远都在视野盒子中，
	// 并且一直在远平面
	gl_Position = gl_Position.xyww;
	// 完美地利用了 1 * 1 *1 立方体顶点坐标的符号，使它们恰好对应于 samplerCube 所需的六个方向
	uvw = aPos;
}