#pragma once

#include <glad/glad.h>

#include <array>

class Geometry
{
public:
	Geometry();
	~Geometry();

	// 创建三角形
	static Geometry* CreateTriangle(const std::array<float,9>& arrPos, const std::array<float,9>& arrColor, const std::array<float,9>& arrUV, const std::array<unsigned int,3>& arrIndices);
	// 创建立方体，立方体中心在原点 
	static Geometry* CreateBox(float size);
	// 创建球体，球体中心在原点
	static Geometry* CreateSphere(float radius);

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
	// 颜色坐标vbo
	GLuint m_colorVbo{ 0 };
	// 顶点uv坐标vbo
	GLuint m_uvVbo{ 0 };
	// 元素缓冲对象/索引缓冲对象，用来存储顶点绘制顺序索引号
	GLuint m_ebo{ 0 };
	// 绘制索引个数
	int m_indicesCount{ 0 };
};