#version 460 core
out vec4 FragColor;
in vec3 color;
in vec2 uv;
uniform sampler2D sampler;
uniform float width;
uniform float height;
void main()
{
	// vec2 - 包含2个float分量的向量
	// textureWidth - 纹理宽度
	// textureHeight - 纹理高度
	// uv - 插值得到的纹理坐标[0,1]^2
	// 计算得出
	// location - 实际纹理坐标[x[0,textureWidth-1], y[0,textureHeight-1]]
	vec2 location = uv * vec2(width, height);;

	// 以location当前像素为中心，向它的水平方向跨出一步(2x2片元)对应的像素location'，可以计算出location处的偏导数(变化率)
	// 本质：在屏幕上水平移动1步，纹理上水平移动了多少步(偏导数，变化率，以纹理像素为单位)，它表示纹理坐标在u和v两个方向上同时发生的变化率。如果斜着贴纹理，u和v方向上的变化率都存在。
	vec2 dx = dFdx(location);
	// 同上
	vec2 dy = dFdy(location);

	// dot(dx,dx) - |dx|^2 = dx.x^2 + dx.y^2 - 向量到标量，为什么要dot呢，有可能同时有u和v两个方向上的变化率，点积目的把方向扔掉，只留大小
	// dot(dy,dy) - |dy|^2 = dy.x^2 + dy.y^2 - 向量到标量，同上
	// 找出最大
	// 开方 - 得到最大的变化率
	// 本质：当前屏幕像素移动1步(水平或垂直的方向)，在纹理空间(水平或者垂直的方向)中 覆盖的最长边有多长 (最大变化率，以纹理像素为单位)
	float maxDelta = sqrt(max(dot(dx,dx), dot(dy,dy)));
	// 取对数 - 得到LOD(Level of Detail)
	float L = log2(maxDelta);

	// maxDelta < 1 => L < 0，取LOD0 => 屏幕像素 > 纹理像素，纹理图片被放大，那就选择该纹理，GL_LINEAR滤波方式
	// 1 < maxDelta < 2 => 0 < L < 1，L > 0.5，就取LOD1，L < 0.5，就取LOD0。
	// 
	// maxDelta = 2 => L = 1, 取LOD1 => 屏幕1x1像素2x2纹理像素，选择LOD1对应纹理
	// ...
	// maxDelta = 8 => L = 3，取LOD3 => 屏幕1x1像素8x8纹理像素，选择LOD3对应纹理
	// 
	// 就上面规范，L大于xxx.5, 就取xxx+1，否则就取xxx
	int level = max(int(L + 0.5), 0);
	FragColor = textureLod(sampler, uv, level);
}