#pragma once

#include <glm/glm.hpp>

class Camera 
{
public:
	Camera();
	virtual ~Camera();

	// 获取视图变化矩阵
	glm::mat4 GetViewMatrix();
	// 获取投影矩阵
	virtual glm::mat4 GetProjectionMatrix() = 0;
	// 摄像机缩放
	virtual void Scale(float deltaScale) = 0;

public:
	// 默认相机在右手坐标系下，+Z轴5个单位长度的位置，向下看
	// 摄像机在世界坐标系的位置
	glm::vec3 m_position{ 0.0f, 0.0f, 5.0f };
	// 摄像机本地坐标系Y轴朝向
	glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
	// 摄像机本地坐标系X轴朝向
	glm::vec3 m_right{ 1.0f, 0.0f, 0.0f };
};