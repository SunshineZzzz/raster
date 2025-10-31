#pragma once

#include "Texture.h"

class FrameBuffer 
{
public:
	FrameBuffer(unsigned int width, unsigned int height);
	~FrameBuffer();

public:
	// 帧缓冲宽高
	unsigned int m_width{ 0 };
	unsigned int m_height{ 0 };
	// 帧缓冲fbo
	unsigned int m_fbo{ 0 };
	// 颜色附件RBO
	Texture* m_colorAttachment{ nullptr };
	// 深度模板附件RBO
	Texture* m_depthStencilAttachment{ nullptr };
};