#pragma once

#include "CameraControl.h"

class GameCameraControl :public CameraControl 
{
public:
	GameCameraControl();
	~GameCameraControl();

	// 鼠标移动事件
	virtual void OnCursor(float xPos, float yPos) override;
	// 每一帧渲染之前都要进行调用，每一帧更新的行为可以放在这里
	virtual void Update() override;

	// 设置wasd移动速度
	void SetSpeed(float s) { m_speed = s; }

private:
	// 上下摇摆摄像机
	void pitch(float angle);
	// 左右摇摆摄像机
	void yaw(float angle);

private:
	// 上次上下要拍的角度
	float m_pitch{ 0.0f };
	// wasd移动速度
	float m_speed{ 0.1f };
};