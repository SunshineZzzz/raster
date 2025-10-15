#include "../inc/Geometry.h"

#include <glm/gtc/constants.hpp>

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

Geometry* Geometry::CreateBox(float size)
{
	Geometry* geometry = new Geometry();
	// 6 * 3 * 2 = 36
	geometry->m_indicesCount = 36;

	float halfSize = size / 2.0f;

	// 一个平面定点数是4个，构成2个三角形，面与面不可以公用，原因是，立方体每个平面的法线一致
	// 6 * 4 * 3 = 72，立方体中心在原点 
	float positions[] = 
	{
		// Front face
		-halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize,
		// Back face
		-halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize,
		// Top face
		-halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize,
		// Bottom face
		-halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize,
		// Right face
		halfSize, -halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize,
		// Left face
		-halfSize, -halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize
	};

	float colors[] =
	{
		// Front (Red)
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		// Back (Green)
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		// Top (Blue)
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		// Bottom (Yellow)
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		// Right (Cyan)
		0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		// Left (Magenta)
		1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};

	float uvs[] = 
	{
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
	};

	unsigned int indices[] = 
	{
		0, 1, 2, 2, 3, 0,   // Front face
		4, 5, 6, 6, 7, 4,   // Back face
		8, 9, 10, 10, 11, 8,  // Top face
		12, 13, 14, 14, 15, 12, // Bottom face
		16, 17, 18, 18, 19, 16, // Right face
		20, 21, 22, 22, 23, 20  // Left face
	};

	glGenBuffers(1, &geometry->m_posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &geometry->m_vao);
	glBindVertexArray(geometry->m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->m_ebo);

	glBindVertexArray(0);

	return geometry;
}

Geometry* Geometry::CreateSphere(float radius)
{
	Geometry* geometry = new Geometry();

	std::vector<GLfloat> positions{};
	std::vector<GLfloat> colors{};
	std::vector<GLfloat> uvs{};
	std::vector<GLuint> indices{};

	// 声明纬线与经线的数量
	int numLatLines = 60; // 纬线
	int numLongLines = 60; // 经线

	// 通过两层循环(纬线在外,经线在内)求出pos和uv
	for (int i = 0; i <= numLatLines; i++) 
	{
		for (int j = 0; j <= numLongLines; j++) 
		{
			float phi = i * glm::pi<float>() / numLatLines;
			float theta = j * 2 * glm::pi<float>() / numLongLines;

			float y = radius * (float)cos(phi);
			float x = radius * (float)sin(phi) * (float)cos(theta);
			float z = radius * (float)sin(phi) * (float)sin(theta);

			positions.push_back(x);
			positions.push_back(y);
			positions.push_back(z);

			float r = (float)i / (float)numLatLines;
			float g = (float)j / (float)numLongLines;
			float b = 1.0f - r;

			colors.push_back(r);
			colors.push_back(g);
			colors.push_back(b);

			float u = 1.0f - (float)j / (float)numLongLines;
			float v = 1.0f - (float)i / (float)numLatLines;

			uvs.push_back(u);
			uvs.push_back(v);
		}
	}


	// 通过两层循环(这里没有=号)求出顶点索引
	for (int i = 0; i < numLatLines; i++) 
	{
		for (int j = 0; j < numLongLines; j++) 
		{
			int p1 = i * (numLongLines + 1) + j;
			int p2 = p1 + numLongLines + 1;
			int p3 = p1 + 1;
			int p4 = p2 + 1;

			indices.push_back(p1);
			indices.push_back(p2);
			indices.push_back(p3);

			indices.push_back(p3);
			indices.push_back(p2);
			indices.push_back(p4);
		}
	}

	glGenBuffers(1, &geometry->m_posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_posVbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_colorVbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_uvVbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &geometry->m_vao);
	glBindVertexArray(geometry->m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->m_ebo);

	glBindVertexArray(0);

	geometry->m_indicesCount = (int)indices.size();

	return geometry;
}