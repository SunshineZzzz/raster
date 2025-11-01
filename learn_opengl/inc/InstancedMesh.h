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
	}

	~InstancedMesh()
	{
		if (m_instanceMatrices != nullptr) 
		{
			delete[] m_instanceMatrices;
			m_instanceMatrices = nullptr;
		}
	}

public:
	// 几何+材质相同对象实例个数
	unsigned int m_instanceCount{ 0 };
	// 每个实例的模型变化矩阵
	glm::mat4* m_instanceMatrices{ nullptr };
};