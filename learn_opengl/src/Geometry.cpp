#include "../inc/Geometry.h"

#include <glm/gtc/constants.hpp>

#include <vector>

Geometry::Geometry() {}

Geometry::Geometry(
	const std::vector<float>& positions,
	const std::vector<float>& normals,
	const std::vector<float>& uvs,
	const std::vector<unsigned int>& indices
) 
{
	m_indicesCount = int(indices.size());

	glGenBuffers(1, &m_posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvVbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalVbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	glBindVertexArray(0);
}


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

	if (glIsBuffer(m_normalVbo))
	{
		glDeleteBuffers(1, &m_normalVbo);
		m_normalVbo = 0;
	}

	if (glIsBuffer(m_ebo))
	{
		glDeleteBuffers(1, &m_ebo);
		m_ebo = 0;
	}
}

Geometry* Geometry::CreateBox(float size)
{
	Geometry* geometry = new Geometry();
	geometry->m_indicesCount = 36;

	float halfSize = size / 2.0f;

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

	float uvs[] = 
	{
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
	};

	float normals[] = 
	{
		// 前面
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		// 后面
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		// 上面
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		// 下面
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		// 右面
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		// 左面
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
	};

	unsigned int indices[] = 
	{
		0, 1, 2, 2, 3, 0,   // Front face
		4, 5, 6, 6, 7, 4,   // Back face
		8, 9, 10, 10, 11, 8,  // Top face
		12, 13, 14, 14, 15, 12, // Bottom face
		16, 17, 18, 18, 19,  16, // Right face
		20, 21, 22, 22, 23, 20  // Left face
	};

	glGenBuffers(1, &geometry->m_posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &geometry->m_vao);
	glBindVertexArray(geometry->m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->m_ebo);

	glBindVertexArray(0);

	return geometry;
}

Geometry* Geometry::CreateSphere(float radius)
{
	Geometry* geometry = new Geometry();

	std::vector<GLfloat> positions{};
	std::vector<GLfloat> uvs{};
	std::vector<GLfloat> normals{};
	std::vector<GLuint> indices{};

	int numLatLines = 60;
	int numLongLines = 60;

	for (int i = 0; i <= numLatLines; i++) {
		for (int j = 0; j <= numLongLines; j++) {
			float phi = i * glm::pi<float>() / numLatLines;
			float theta = j * 2 * glm::pi<float>() / numLongLines;

			float y = float(radius * cos(phi));
			float x = float(radius * sin(phi) * cos(theta));
			float z = float(radius * sin(phi) * sin(theta));

			positions.push_back(x);
			positions.push_back(y);
			positions.push_back(z);

			float u = float(1.0 - (float)j / (float)numLongLines);
			float v = float(1.0 - (float)i / (float)numLatLines);

			uvs.push_back(u);
			uvs.push_back(v);

			normals.push_back(x);
			normals.push_back(y);
			normals.push_back(z);
		}
	}

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

	glGenBuffers(1, &geometry->m_uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_uvVbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_normalVbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &geometry->m_vao);
	glBindVertexArray(geometry->m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->m_ebo);

	glBindVertexArray(0);

	geometry->m_indicesCount = int(indices.size());

	return geometry;
}

Geometry* Geometry::CreatePlane(float width, float height)
{
	Geometry* geometry = new Geometry();
	geometry->m_indicesCount = 6;

	float halfW = width / 2.0f;
	float halfH = height / 2.0f;

	float positions[] = 
	{
		-halfW, -halfH, 0.0f,
		halfW, -halfH, 0.0f,
		halfW, halfH, 0.0f,
		-halfW, halfH, 0.0f,
	};

	float uvs[] = 
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	float normals[] = 
	{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	unsigned int indices[] = 
	{
		0, 1, 2,
		2, 3, 0
	};


	glGenBuffers(1, &geometry->m_posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &geometry->m_vao);
	glBindVertexArray(geometry->m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->m_ebo);

	glBindVertexArray(0);

	return geometry;
}

Geometry* Geometry::CreateScreenPlane()
{
	Geometry* geometry = new Geometry();
	geometry->m_indicesCount = 6;

	// 构建数据positions uv
	float positions[] = 
	{
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f,
	};

	float uvs[] = 
	{
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	unsigned int indices[] = 
	{
		0, 1, 2,
		0, 2, 3
	};

	// 创建vao vbo等
	glGenBuffers(1, &geometry->m_posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, geometry->m_uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->m_ebo);

	glBindVertexArray(0);

	return geometry;
}