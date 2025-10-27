#pragma once 

#include <glm/glm.hpp>

class Object 
{
public:
	Object();
	virtual ~Object();

	// 设置世界坐标系位置
	void SetPosition(glm::vec3 pos);
	// 增量绕自身X/Y/Z轴旋转
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);
	// 缩放
	void SetScale(glm::vec3 scale);
	// 获取模型矩阵
	glm::mat4 GetModelMatrix();

protected:
	// 世界坐标系位置
	glm::vec3 m_position{ 0.0f };
	// 绕自身X/Y/Z轴旋转角度
	// pitch
	float m_angleX{ 0.0f };
	// yaw
	float m_angleY{ 0.0f };
	// roll
	float m_angleZ{ 0.0f };
	// 缩放
	glm::vec3 m_scale{ 1.0f };
};