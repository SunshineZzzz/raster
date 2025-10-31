#version 460 core
out vec4 FragColor;
in vec2 uv;
uniform sampler2D screenTexSampler;
uniform float texWidth;
uniform float texHeight;

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

// 模糊
vec3 Blur()
{	
	// 相邻两个像素的u差值
	float du = 1.0 / texWidth;
	// 相邻两个像素的v差值
	float dv = 1.0 / texHeight;

	// 偏移值数组，是一个3X3矩阵，3个为一行
	vec2 offsets[9] = vec2[] 
	(
		// 左上
		vec2(-du, dv),
		// 正上
		vec2(0.0, dv),
		// 右上
		vec2(du, dv),
		// 左中
		vec2(-du, 0.0),
		// 中
		vec2(0.0, 0.0),
		// 右中
		vec2(du, 0.0),
		// 左下
		vec2(-du, -dv),
		// 正下
		vec2(0.0, -dv),
		// 右下
		vec2(du, -dv)
	);

	// 卷积核矩阵，是一个3X3矩阵，3个为一行
	// 计算中间新的颜色，加权周边再平均
	float kernel[9] = float[]
	(
		1.0, 2.0, 1.0,
		2.0, 4.0, 2.0,
		1.0, 2.0, 1.0
	);

	// 加权相加
	vec3 sumColor = vec3(0.0);
	for(int i = 0; i < 9; i++)
	{
		vec3 sampleColor = texture(screenTexSampler, uv + offsets[i]).rgb;
		sumColor += sampleColor * kernel[i];
	}
	// 再平均
	sumColor /= 16.0;

	return sumColor;
}

void main()
{
	// vec3 color = ColorInvert(texture(screenTexSampler, uv).rgb);
	// vec3 color = Gray(texture(screenTexSampler, uv).rgb);
	// vec3 color = GrayCorrect(texture(screenTexSampler, uv).rgb);
	vec3 color = Blur();

	FragColor = vec4(color, 1.0);
	// FragColor = texture(screenTexSampler, uv);
}