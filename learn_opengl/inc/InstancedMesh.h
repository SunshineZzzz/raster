#pragma once 

#include "Mesh.h"
#include <algorithm>
#include <vector>

class InstancedMesh : public Mesh 
{
public:
	InstancedMesh(Geometry* geometry, Material* material, unsigned int instanceCount) :
		Mesh(geometry, material)
	{
		m_type = ObjectType::InstancedMesh;
		m_instanceCount = instanceCount;
		m_instanceMatrices.resize(instanceCount);

		glGenBuffers(1, &m_matrixVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_matrixVbo);
		// 这里数据是空的，只是开辟了显存
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * m_instanceCount, m_instanceMatrices.data(), GL_DYNAMIC_DRAW);;

		glBindVertexArray(m_geometry->GetVao());
		glBindBuffer(GL_ARRAY_BUFFER, m_matrixVbo);

		for (int i = 0; i < 4; i++) 
		{
			glEnableVertexAttribArray(4 + i);
			glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, 
				sizeof(glm::mat4), (void*)(sizeof(float) * i * 4));
			// 这个属性每个实例化更新一次
			glVertexAttribDivisor(4 + i, 1);
		}

		glBindVertexArray(0);
	}

	~InstancedMesh()
	{
	}

	// 正式写入数据到显存空间
	void UpdateMatrices()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_matrixVbo);
		// 如果使用glBufferData进行数据更新，会导致重新分配显存空间
		// glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * m_instanceCount, m_instanceMatrices, GL_DYNAMIC_DRAW);;
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * m_instanceCount, m_instanceMatrices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// 排序，由远及近排序
	void SortMatrices(glm::mat4 viewMatrix)
	{
		std::sort(
			m_instanceMatrices.begin(),
			m_instanceMatrices.end(),
			[viewMatrix](const glm::mat4& a, const glm::mat4& b) {

				//1 计算a的相机系的Z
				auto modelMatrixA = a;
				auto worldPositionA = modelMatrixA * glm::vec4(0.0, 0.0, 0.0, 1.0);
				auto cameraPositionA = viewMatrix * worldPositionA;

				//2 计算b的相机系的Z
				auto modelMatrixB = b;
				auto worldPositionB = modelMatrixB * glm::vec4(0.0, 0.0, 0.0, 1.0);
				auto cameraPositionB = viewMatrix * worldPositionB;

				return cameraPositionA.z < cameraPositionB.z;
			}
		);
	}

public:
	// 几何+材质相同对象实例个数
	unsigned int m_instanceCount{ 0 };
	// 每个实例的模型变化矩阵
	// 也会出现透明问题排序问题，以前解决透明问题实例化，咱们自己排序，由远及近画。前面是多次调用drawcall。
	// 这里是调用一次drawcall，但是根据模型矩阵不同，画出来多个实例。所以排序问题还是得自己解决。
	// opengl在vs中绘制的时候是按照实例顺序去绘的，0~m_instanceCount取得m_instanceMatrices[i]。
	// 让m_instanceMatrices数组进行排序，由远及近排序(摄像机坐标系下)即可。
	std::vector<glm::mat4> m_instanceMatrices;
	// 实例化矩阵VBO
	unsigned int m_matrixVbo{ 0 };
};