#pragma once

#include <glad/glad.h>

#include <array>
#include <vector>

class Geometry
{
public:
	Geometry();
	Geometry(
		const std::vector<float>& positions,
		const std::vector<float>& normals,
		const std::vector<float>& uvs,
		const std::vector<unsigned int>& indices
	);
	~Geometry();

	// 创建立方体，立方体中心在原点 
	static Geometry* CreateBox(float size);
	// 创建球体，球体中心在原点
	static Geometry* CreateSphere(float radius);
	// 创建平面，平面中心在原点
	static Geometry* CreatePlane(float width, float height);

	// 获取VAO
	GLuint GetVao()const { return m_vao; }
	// 获取绘制索引个数
	int GetIndicesCount()const { return m_indicesCount; }

private:
	// 顶点数组对象，用来存储一个Mesh网格所有的顶点属性描述信息
	GLuint m_vao{ 0 };
	// 顶点缓冲对象，用来存储顶点属性数据
	// 顶点坐标vbo
	GLuint m_posVbo{ 0 };
	// 顶点uv坐标vbo
	GLuint m_uvVbo{ 0 };
	// 顶点法线vbo
	GLuint m_normalVbo{ 0 };
	// 元素缓冲对象/索引缓冲对象，用来存储顶点绘制顺序索引号
	GLuint m_ebo{ 0 };
	// 绘制索引个数
	int m_indicesCount{ 0 };
};