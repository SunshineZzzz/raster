#include "../inc/FrameBuffer.h"

#include <cassert>

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;

	// 生成fbo对象并且绑定
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);


	// 生成颜色附件，并且加入fbo
	m_colorAttachment = Texture::CreateColorAttachment(m_width, m_height, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment->GetTexture(), 0);

	// 生成depth Stencil附件，加入fbo
	m_depthStencilAttachment = Texture::CreateDepthStencilAttachment(m_width, m_height, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthStencilAttachment->GetTexture(), 0);

	// 检查当前构建的fbo是否完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{
		assert(0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	if (m_fbo) 
	{
		glDeleteFramebuffers(1, &m_fbo);
	}

	if (m_colorAttachment != nullptr) {
		delete m_colorAttachment;
	}

	if (m_depthStencilAttachment != nullptr) 
	{
		delete m_depthStencilAttachment;
	}
}