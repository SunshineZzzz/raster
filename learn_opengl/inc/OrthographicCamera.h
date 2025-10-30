#pragma once

#include "camera.h"

class OrthographicCamera : public Camera 
{
public:
	OrthographicCamera(float l, float r, float t, float b, float n, float f);
	virtual ~OrthographicCamera() override; ;

	// 获取投影矩阵
	glm::mat4 GetProjectionMatrix() override;
	// 摄像机缩放
	void Scale(float deltaScale) override;

private:
	// 投影盒子定义
	// 左边界
	float m_left = 0.0f;
	// 右边界
	float m_right = 0.0f;
	// 上边界
	float m_top = 0.0f;
	// 下边界
	float m_bottom = 0.0f;
	// 摄像机缩放比例
	float m_scale{ 0.0f };
};