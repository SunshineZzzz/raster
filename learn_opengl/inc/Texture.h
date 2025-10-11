#pragma once

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include <string>

class Texture 
{
public:
	Texture(const std::string& path, unsigned int unit);
	~Texture();

	// 纹理单元与纹理对象绑定
	void Bind();
	// 是否已经初始化过
	bool IsInitialized() const;

private:
	// 纹理对象
	GLuint m_texture{ 0 };
	// 纹理单元号
	unsigned int m_unit{ 0 };
	// 是否已经初始化过
	bool m_initialized = false;

public:
	// 纹理宽高
	int m_width{ 0 };
	int m_height{ 0 };
};