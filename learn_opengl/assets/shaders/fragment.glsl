#version 460 core
out vec4 FragColor;
in vec2 uv;
in vec3 normal;
uniform sampler2D sampler;
// 光源参数，方向+颜色
uniform vec3 lightDirection;
uniform vec3 lightColor;
void main()
{
	// 获取物体的当前像素的颜色
	vec3 objectColor  = texture(sampler, uv).xyz;
	// 准备diffuse(漫反射)相关的各类数据
	vec3 normalN = normalize(normal);
	vec3 lightDirN = normalize(lightDirection);
	// dot(-lightDirN, normalN) 光源与法线的夹角的余弦值
	// clamp限制在0.0~1.0之间
	float diffuse = clamp(dot(-lightDirN, normalN), 0.0, 1.0);
	// 计算出漫反射物体颜色
	vec3 finalColor = lightColor * diffuse * objectColor;
	FragColor = vec4(finalColor, 1.0);
}