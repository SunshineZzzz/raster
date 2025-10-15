#include "../inc/Geometry.h"

#include <vector>

Geometry::Geometry() {}

Geometry::~Geometry() 
{
	if (glIsVertexArray(m_vao))
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}

	if (glIsBuffer(m_posVbo))
	{
		glDeleteBuffers(1, &m_posVbo);
		m_posVbo = 0;
	}

	if (glIsBuffer(m_uvVbo))
	{
		glDeleteBuffers(1, &m_uvVbo);
		m_uvVbo = 0;
	}

	if (glIsBuffer(m_colorVbo))
	{
		glDeleteBuffers(1, &m_colorVbo);
		m_colorVbo = 0;
	}

	if (glIsBuffer(m_ebo))
	{
		glDeleteBuffers(1, &m_ebo);
		m_ebo = 0;
	}
}

Geometry* Geometry::CreateTriangle(const std::array<float,9>& arrPos, const std::array<float,9>& arrColor, const std::array<float,9>& arrUV, const std::array<unsigned int,3>& arrIndices)
{
	Geometry* geometry = new Geometry();
	geometry->m_indicesCount = int(arrIndices.size());

	auto a = arrPos.data();

	// 创建VBO
	glGenBuffers(1, &geometry->m_posVbo);
	// 绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_posVbo);
	// 上传数据
	glBufferData(GL_ARRAY_BUFFER, arrPos.size() * sizeof(float), arrPos.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_colorVbo);
	glBufferData(GL_ARRAY_BUFFER, arrColor.size() * sizeof(float), arrColor.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_uvVbo);
	glBufferData(GL_ARRAY_BUFFER, arrUV.size() * sizeof(float), arrUV.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrIndices.size() * sizeof(unsigned int), arrIndices.data(), GL_STATIC_DRAW);

	// 创建VAO
	glGenVertexArrays(1, &geometry->m_vao);
	// 绑定VAO
	glBindVertexArray(geometry->m_vao);

	// 加入位置属性信息给VAO
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	// 加入颜色属性信息给VAO
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	// 加入纹理坐标属性信息给VAO
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	// 加入索引信息给VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->m_ebo);

	// 解绑VAO
	glBindVertexArray(0);

	return geometry;
}