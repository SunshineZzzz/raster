#version 460 core
out vec4 FragColor;
in vec2 uv;
in vec3 normal;
in vec3 worldPosition;
// diffuse贴图采样器
uniform sampler2D sampler;
// specularMask贴图采样器
// uniform sampler2D specularMaskSampler;
// opacityMask贴图采样器
uniform sampler2D opacityMaskSampler;
// 环境光颜色
uniform vec3 ambientColor;
// 相机世界位置
uniform vec3 cameraPosition;
// 镜面反射光斑大小
// uniform float shiness;
// 平行光
struct DirectionalLight
{
	// 光照方向
	vec3 direction;
	// 光照颜色
	vec3 color;
	// 镜面反射光斑亮度
	float specularIntensity;
};
// 点光源
struct PointLight
{
	// 点光源位置
	vec3 position;
	// 点光源颜色
	vec3 color;
	// 镜面反射光斑亮度
	float specularIntensity;
	// 衰减系数
	float k2;
	float k1;
	float kc;
};
// 聚光灯
struct SpotLight
{
	// 聚光灯位置
	vec3 position;
	// 聚光灯朝向的方向
	vec3 targetDirection;
	// 聚光灯颜色
	vec3 color;
	// 光锥体张开角度，边缘可视区
	float outerLine;
	// 光锥体张开角度，绝对可视区
	float innerLine;
	// 镜面反射光斑亮度
	float specularIntensity;
};
// 聚光灯
uniform SpotLight spotLight;
// 平行光
uniform DirectionalLight directionalLight;
// 宏定义
#define POINT_LIGHT_NUM 4
uniform PointLight pointLights[POINT_LIGHT_NUM];
// 透明度
uniform float opacity;

// 计算漫反射光照
vec3 CalculateDiffuse(vec3 lightColor, vec3 objectColor, vec3 lightDir, vec3 normal)
{
	// 计算diffuse(漫反射)
	// dot(-lightDirN, normalN)光源与法线的夹角的余弦值
	// clamp限制在0.0~1.0之间
	float diffuse = clamp(dot(-lightDir, normal), 0.0, 1.0);
	vec3 diffuseColor = lightColor * diffuse * objectColor;

	return diffuseColor;
}

// 计算镜面反射光照
vec3 CalculateSpecular(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float intensity)
{
	// 计算specular(高光反射，镜面反射)
	// 防止背面光效果，dotResult大于0.0说明是正面照射，小于0.0说明背面照射
	float dotResult = dot(-lightDir, normal);
	// step函数，如果dotResult大于0.0，返回1.0，否则返回0.0
	float flag = step(0.0, dotResult);
	// 反射光方向
	vec3 lightReflect = normalize(reflect(lightDir, normal));
	// 观察方向与反射方向夹角的余弦值，为1的时候就重合了，最亮
	float specular = max(dot(lightReflect, -viewDir), 0.0);
	// 控制光斑大小，越打越小
	// specular = pow(specular, shiness);
	// specularMaskSampler对应的纹理对象中可以拿到r，最终让某些地方很亮
	// float specularMask = texture(specularMaskSampler, uv).r;
	// 不需要计算objectColor，镜面反射，应该不用考虑物体吸收把，我觉的
	vec3 specularColor = lightColor * specular * flag * intensity;

	return specularColor;
}

// 聚光灯
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
	// 计算光照的通用数据
	// 对象颜色(插值)
	vec3 objectColor  = texture(sampler, uv).xyz;
	// 聚光灯照射物体方向
	vec3 lightDir = normalize(worldPosition - light.position);
	// 聚光灯朝向的方向
	vec3 targetDir = normalize(light.targetDirection);
	// 计算spotlight的照射范围
	// 聚光灯照射物体方向和聚光灯朝向的方向夹角的余弦值
	float cGamma = dot(lightDir, targetDir);
	// 具体看md，其实就是一个边缘模糊值
	float intensity =clamp((cGamma-light.outerLine)/(light.innerLine-light.outerLine), 0.0, 1.0);

	// 计算diffuse, 计算漫反射光照
	vec3 diffuseColor = CalculateDiffuse(light.color, objectColor, lightDir, normal);

	// 计算specular, 计算镜面反射光照
	vec3 specularColor = CalculateSpecular(light.color, lightDir,normal, viewDir, light.specularIntensity); 

	return (diffuseColor + specularColor) * intensity;
}

// 平行光
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal ,vec3 viewDir)
{
	// 计算光照的通用数据
	// 对象颜色(插值)
	vec3 objectColor  = texture(sampler, uv).xyz;
	// 平行光方向
	vec3 lightDir = normalize(light.direction);

	// 计算diffuse, 计算漫反射光照
	vec3 diffuseColor = CalculateDiffuse(light.color, objectColor, lightDir, normal);

	// 计算specular, 计算镜面反射光照
	// vec3 specularColor = CalculateSpecular(light.color, lightDir,normal, viewDir, light.specularIntensity); 

	// return diffuseColor + specularColor;
	return diffuseColor;
}

// 点光源
vec3 CalculatePointLight(PointLight light, vec3 normal ,vec3 viewDir)
{
	// 计算光照的通用数据
	// 对象颜色(插值)
	vec3 objectColor  = texture(sampler, uv).xyz;
	// 点光源照射物体方向
	vec3 lightDir = normalize(worldPosition - light.position);

	// 计算衰减
	float dist = length(worldPosition - light.position);
	float attenuation = 1.0 / (light.k2 * dist * dist + light.k1 * dist + light.kc);

	// 计算diffuse, 计算漫反射光照
	vec3 diffuseColor = CalculateDiffuse(light.color,objectColor, lightDir, normal);

	// 计算specular, 计算镜面反射光照
	vec3 specularColor = CalculateSpecular(light.color, lightDir,normal, viewDir, light.specularIntensity); 

	return (diffuseColor + specularColor) * attenuation;
}

void main()
{
	vec3 result = vec3(0.0 ,0.0, 0.0);
	// 计算光照的通用数据
	// 对象颜色(插值)
	vec3 objectColor  = texture(sampler, uv).xyz;
	// 材质蒙版透明度
	float alpha =  texture(opacityMaskSampler, uv).r;
	// 对象法线(插值)
	vec3 normalN = normalize(normal);
	// 视线方向
	vec3 viewDir = normalize(worldPosition - cameraPosition);

	// result += CalculateSpotLight(spotLight, normalN, viewDir);
	result += CalculateDirectionalLight(directionalLight,normalN, viewDir);

	// for(int i = 0;i < POINT_LIGHT_NUM; i++) 
    // {
	//		result += CalculatePointLight(pointLights[i], normalN, viewDir);
	// }

	// 环境光计算
	vec3 ambientColor = objectColor * ambientColor;

	vec3 finalColor = result + ambientColor;
 
	FragColor = vec4(finalColor, alpha * opacity);
}