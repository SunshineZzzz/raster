#version 460 core
out vec4 FragColor;
in vec2 uv;
uniform sampler2D screenTexSampler;

// 颜色反相
vec3 ColorInvert(vec3 color)
{
	vec3 invertColor = vec3(1.0) - color;
	return invertColor;
}

// 平均灰度
vec3 Gray(vec3 color)
{
    // 这样平均亮度信息不会丢失
	float avg = (color.r + color.g + color.b) / 3.0;
	return vec3(avg);
}

// 由于人眼对于绿色更加敏感，对蓝色不敏感，为了物理精确
// 可以为每个颜色通道，增加权重值，让蓝色小一些，绿色多一些
// 还是灰色，只不过原来绿色对应的亮度高一些
vec3 GrayCorrect(vec3 color)
{
	float avg = color.r * 0.2126 + color.g * 0.7152 +color.b * 0.0722;
	return vec3(avg);
}

void main()
{
	// vec3 color = ColorInvert(texture(screenTexSampler, uv).rgb);
	// vec3 color = Gray(texture(screenTexSampler, uv).rgb);
	vec3 color = GrayCorrect(texture(screenTexSampler, uv).rgb);

	FragColor = vec4(color, 1.0);
}