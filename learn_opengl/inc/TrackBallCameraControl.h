#pragma once

#include "cameraControl.h"

class TrackBallCameraControl :public CameraControl 
{
public:
	TrackBallCameraControl();
	~TrackBallCameraControl();

	// 鼠标移动事件
	virtual void OnCursor(float xPos, float yPos) override;
	// 滚轮事件
	virtual void OnScroll(float offsetY) override;

private:
	// 经线变化度数
	void pitch(float angle);
	// 纬线变化度数
	void yaw(float angle);

private:
	// 鼠标中间移动速度
	float m_moveSpeed = 0.005f;
};