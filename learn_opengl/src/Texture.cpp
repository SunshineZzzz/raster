#define STB_IMAGE_IMPLEMENTATION 1
#include <stb/stb_image.h>

#include "../inc/texture.h"

std::map<std::string, Texture*> Texture::sTextureCache{};

Texture* Texture::CreateTexture(const std::string& path, unsigned int unit)
{
	// 检查是否缓存过本路径对应的纹理对象
	auto iter = sTextureCache.find(path);
	if (iter != sTextureCache.end()) 
	{
		return iter->second;
	}

	// 如果本路径对应的texture没有生成过，则重新生成
	auto texture = new Texture(path, unit);
	sTextureCache[path] = texture;

	return texture;
}

Texture* Texture::CreateTextureFromMemory(
	const std::string& path,
	unsigned int unit,
	unsigned char* dataIn,
	uint32_t widthIn,
	uint32_t heightIn
)
{
	// 检查是否缓存过本路径对应的纹理对象
	auto iter = sTextureCache.find(path);
	if (iter != sTextureCache.end()) 
	{
		return iter->second;
	}

	// 如果本路径对应的texture没有生成过，则重新生成
	auto texture = new Texture(unit, dataIn, widthIn, heightIn);
	sTextureCache[path] = texture;

	return texture;
}

// 创建颜色附件
Texture* Texture::CreateColorAttachment(
	unsigned int width,
	unsigned int height,
	unsigned int unit
)
{
	return new Texture(width, height, unit);
}

// 创建深度模板附件
Texture* Texture::CreateDepthStencilAttachment(
	unsigned int width,
	unsigned int height,
	unsigned int unit
)
{
	Texture* dsTex = new Texture();

	unsigned int depthStencil;
	glGenTextures(1, &depthStencil);
	glBindTexture(GL_TEXTURE_2D, depthStencil);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	dsTex->m_texture = depthStencil;
	dsTex->m_width = width;
	dsTex->m_height = height;
	dsTex->m_unit = unit;

	return dsTex;
}

Texture::Texture() {}

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

	// 自动生成mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	// 释放数据
	stbi_image_free(data);

	// 设置纹理的过滤方式
	// 采样(Sampling)：动作。是从纹理或场景中读取一个或多个点的数据的过程。
	// 过滤(Filtering)：方法/处理。是根据采样的点和预设的算法（如平均、插值）来计算出一个最终颜色的过程。
	// 
	// GL_TEXTURE_MAG_FILTER (放大过滤)
	// 发生时机：屏幕像素 > 纹理像素 (纹理被拉伸)
	// 描述：屏幕上需要的像素比实际纹理对象像素多，采用线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// GL_TEXTURE_MIN_FILTER (缩小过滤)
	// 发生时机：屏幕像素 < 纹理像素 (纹理被压缩)
	// 描述：屏幕上需要的像素比实际纹理对象像素少，采用临近过滤
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// GL_LINEAR_MIPMAP_LINEAR: 在单个mipmap上采用线性采样，在两层mipmap LOD之间(比如L=1.3,L1,L2之间)采用线性过滤来获取纹理像素
	// GL_NEAREST_MIPMAP_NEAREST: 在单个mipmap上采用临近采样，在两层mipmap LOD之间(比如L=1.3,L1,L2之间)采用临近过滤来获取纹理像素
	// 还有其他的组合方式，比如GL_LINEAR_MIPMAP_NEAREST，GL_NEAREST_MIPMAP_LINEAR等
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	// 设置纹理的包裹方式
	// u纹理坐标超出[0,1]范围，采用重复模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// v纹理坐标超出[0,1]范围，采用重复模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	m_initialized = true;
}

Texture::Texture(unsigned int unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn)
{
	m_unit = unit;

	// stbImage 读取图片
	int channels;
	// 反转y轴
	stbi_set_flip_vertically_on_load(true);
	// 计算整张图片的大小
	// Assimp规定：如果内嵌纹理是png或者jpg压缩格式的话，height = 0，width就代表了图片大小
	uint32_t dataInSize = 0;
	if (!heightIn) 
	{
		dataInSize = widthIn;
	}
	else 
	{
		// 如果内嵌图片不是压缩格式，height = 正常高度，width = 正常宽度
		// 偷懒：统一认为数据格式都是RGBA
		dataInSize = widthIn * heightIn * 4;
	}

	unsigned char* data = stbi_load_from_memory(dataIn, dataInSize, &m_width, &m_height, &channels, STBI_rgb_alpha);

	// 生成纹理并且激活单元绑定
	glGenTextures(1, &m_texture);
	// 激活纹理单元
	glActiveTexture(GL_TEXTURE0 + m_unit);
	// 绑定纹理对象
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// 传输纹理数据, 开辟显存
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	// 释放数据 
	stbi_image_free(data);

	// 设置纹理的过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	// 设置纹理的包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture(unsigned int width, unsigned int height, unsigned int unit)
{
	m_width = width;
	m_height = height;
	m_unit = unit;

	glGenTextures(1, &m_texture);
	glActiveTexture(GL_TEXTURE0 + m_unit);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(const std::vector<std::string>& paths, unsigned int unit)
{
	m_unit = unit;
	m_textureTarget = GL_TEXTURE_CUBE_MAP;

	// cubemap不需要反转y轴，而且一定还要写，这是因为前面加载2D纹理的时候，就反转了，就会出现错误
	// 一定要把状态重置回来，stbi里面也有状态啊
	stbi_set_flip_vertically_on_load(false);

	// 创建CubeMap对象
	glGenTextures(1, &m_texture);
	glActiveTexture(GL_TEXTURE0 + m_unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	// 循环读取六张贴图，并且放置到cubemap的六个GPU空间内
	int channels;
	int width = 0, height = 0;
	unsigned char* data = nullptr;
	for (int i = 0; i < paths.size(); i++) 
	{
		data = stbi_load(paths[i].c_str(), &width, &height, &channels, STBI_rgb_alpha);
		if (data != nullptr) 
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else 
		{
			stbi_image_free(data);
			assert(0);
		}
	}

	// 设置纹理参数，这样子没有接缝
	glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
	// 先激活纹理单元，然后绑定纹理对象
	glActiveTexture(GL_TEXTURE0 + m_unit);
	glBindTexture(m_textureTarget, m_texture);
}

bool Texture::IsInitialized() const
{
	return m_initialized;
}