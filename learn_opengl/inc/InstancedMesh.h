#pragma once 

#include "Mesh.h"

class InstancedMesh : public Mesh 
{
public:
	InstancedMesh(Geometry* geometry, Material* material, unsigned int instanceCount) :
		Mesh(geometry, material)
	{
		m_type = ObjectType::InstancedMesh;
		m_instanceCount = instanceCount;
		m_instanceMatrices = new glm::mat4[instanceCount];

		glGenBuffers(1, &m_matrixVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_matrixVbo);
		// 这里数据是空的，只是开辟了显存
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * m_instanceCount, m_instanceMatrices, GL_DYNAMIC_DRAW);;

		glBindVertexArray(m_geometry->GetVao());
		glBindBuffer(GL_ARRAY_BUFFER, m_matrixVbo);

		for (int i = 0; i < 4; i++) 
		{
			glEnableVertexAttribArray(3 + i);
			glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, 
				sizeof(glm::mat4), (void*)(sizeof(float) * i * 4));
			// 这个属性每个实例化更新一次
			glVertexAttribDivisor(3 + i, 1);
		}

		glBindVertexArray(0);
	}

	~InstancedMesh()
	{
		if (m_instanceMatrices != nullptr) 
		{
			delete[] m_instanceMatrices;
			m_instanceMatrices = nullptr;
		}
	}

	// 正式写入数据到显存空间
	void UpdateMatrices()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_matrixVbo);
		// 如果使用glBufferData进行数据更新，会导致重新分配显存空间
		// glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * m_instanceCount, m_instanceMatrices, GL_DYNAMIC_DRAW);;
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * m_instanceCount, m_instanceMatrices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

public:
	// 几何+材质相同对象实例个数
	unsigned int m_instanceCount{ 0 };
	// 每个实例的模型变化矩阵
	glm::mat4* m_instanceMatrices{ nullptr };
	// 实例化矩阵VBO
	unsigned int m_matrixVbo{ 0 };
};