#version 460 core
out vec4 FragColor;
in vec2 uv;
in vec3 normal;
in vec3 worldPosition;
uniform sampler2D sampler;
// 世界光源参数
uniform vec3 lightDirection;
uniform vec3 lightColor;
// 世界相机位置
uniform vec3 cameraPosition;
void main()
{	
	// 测试
	// 摄像机视线
	vec3 viewDir = normalize(worldPosition - cameraPosition);
	viewDir = clamp(viewDir, 0.0, 1.0);
	FragColor = vec4(viewDir, 1.0);
}