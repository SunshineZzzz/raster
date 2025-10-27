#version 460 core
out vec4 FragColor;
in vec2 uv;
in vec3 normal;
in vec3 worldPosition;
uniform sampler2D sampler;
// 光源参数
uniform vec3 lightDirection;
uniform vec3 lightColor;
// 相机世界位置
uniform vec3 cameraPosition;
// 光斑亮的程度
uniform float specularIntensity;
// 环境光颜色
uniform vec3 ambientColor;
void main()
{
	// 计算光照的通用数据
	// 对象颜色(插值)
	vec3 objectColor  = texture(sampler, uv).xyz;
	// 对象法线(插值)
	vec3 normalN = normalize(normal);
	// 平行光方向
	vec3 lightDirN = normalize(lightDirection);
	// 视线方向
	vec3 viewDir = normalize(worldPosition - cameraPosition);

	// 计算diffuse(漫反射)
	// dot(-lightDirN, normalN)光源与法线的夹角的余弦值
	// clamp限制在0.0~1.0之间
	float diffuse = clamp(dot(-lightDirN, normalN), 0.0, 1.0);
	vec3 diffuseColor = lightColor * diffuse * objectColor;
	
	// 计算specular(高光反射，镜面反射)
	// 防止背面光效果，dotResult大于0说明是正面照射，小于0说明背面照射
	float dotResult = dot(-lightDirN, normalN);
	// step函数，如果dotResult大于0.0，返回1.0，否则返回0.0
	float flag = step(0.0, dotResult);
	// 反射光方向
	vec3 lightReflect = normalize(reflect(lightDirN, normalN));
	// 观察方向与反射方向夹角的余弦值，为1的时候就重合了，最亮
	float specular = max(dot(lightReflect, -viewDir), 0.0);
	// 控制光斑大小
	specular = pow(specular, 64);
	// 不需要计算objectColor，镜面反射，应该不用考虑物体吸收把，我觉的
	vec3 specularColor = lightColor * specular * flag * specularIntensity;

	// 环境光计算，前面两种反射会造成物体没有接收到光照的地方是黑色，解决黑色问题
	vec3 ambientColor = objectColor * ambientColor;

	// 最终颜色
	vec3 finalColor = diffuseColor + specularColor + ambientColor;

	FragColor = vec4(finalColor, 1.0);
}