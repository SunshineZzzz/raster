#define STB_IMAGE_IMPLEMENTATION 1
#include <stb/stb_image.h>

#include "../inc/texture.h"

Texture::Texture(const std::string& path, unsigned int unit) 
{
	m_unit = unit;

	int channels;
	// 反转y轴
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &channels, STBI_rgb_alpha);
	if (!data)
	{
		SDL_Log("stbi_load error,%s,%s", stbi_failure_reason(), path.c_str());
		return;
	}

	// 创建纹理对象
	glGenTextures(1, &m_texture);
	// 激活纹理单元
	glActiveTexture(GL_TEXTURE0 + m_unit);
	// 绑定纹理对象
	glBindTexture(GL_TEXTURE_2D, m_texture);
	// 纹理对象m_texture就被对应到了纹理单元GL_TEXTURE0+m_unit
	// 开辟显存，并上传数据
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// 释放数据
	stbi_image_free(data);

	// 设置纹理的过滤方式
	// GL_TEXTURE_MAG_FILTER (放大过滤)
	// 发生时机：屏幕像素 > 纹理像素 (纹理被拉伸)
	// 描述：屏幕上需要的像素比实际纹理对象像素多，采用线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// GL_TEXTURE_MIN_FILTER (缩小过滤)
	// 发生时机：屏幕像素 < 纹理像素 (纹理被压缩)
	// 描述：屏幕上需要的像素比实际纹理对象像素少，采用临近过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// 设置纹理的包裹方式
	// u纹理坐标超出[0,1]范围，采用重复模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// v纹理坐标超出[0,1]范围，采用重复模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	m_initialized = true;
}

Texture::~Texture() 
{
	if (!m_initialized)
	{
		return;
	}

	if (glIsTexture(m_texture))
	{
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}
}

void Texture::Bind() 
{
	// 先切换纹理单元，然后绑定纹理对象
	glActiveTexture(GL_TEXTURE0 + m_unit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

bool Texture::IsInitialized() const
{
	return m_initialized;
}