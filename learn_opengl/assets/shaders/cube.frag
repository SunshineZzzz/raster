#version 460 core
out vec4 FragColor;
in vec3 uvw;
uniform samplerCube cubeSampler;
void main()
{
	// 片段着色器 (Fragment Shader) 的主要输出颜色
	// 还要经历：模板测试，深度测试，颜色混合，最终写入颜色缓冲区，显示在屏幕
	// 进行天空盒颜色采样
	FragColor = texture(cubeSampler, uvw);
}