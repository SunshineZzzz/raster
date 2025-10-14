#pragma once

#include "camera.h"

class PerspectiveCamera :public Camera 
{
public:
	PerspectiveCamera(float fovy, float aspect, float near, float far);
	~PerspectiveCamera() override;

	// 获取投影矩阵
	glm::mat4 GetProjectionMatrix() override;
	// 摄像机缩放比例
	void Scale(float deltaScale) override;

private:
	// 视锥体定义
	// +y轴方向视张角
	float m_fovy = 0.0f;
	// 近平面横纵比
	float m_aspect = 0.0f;
	// 近平面
	float m_near = 0.0f;
	// 远平面
	float m_far = 0.0f;
};