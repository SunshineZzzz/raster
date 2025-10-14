#pragma once

#include "camera.h"

#include <map>
#include <memory>

class CameraControl 
{
public:
	CameraControl();
	virtual ~CameraControl();

	// 鼠标事件
	virtual void OnMouse(unsigned char button, bool bDownOrUp, float xPos, float yPos);
	// 鼠标移动事件
	virtual void OnCursor(float xPos, float yPos);
	// 键盘事件
	virtual void OnKey(unsigned key, bool bDownOrUp, bool repeat);
	// 滚轮事件
	virtual void OnScroll(float offsetY);
	// 每一帧渲染之前都要进行调用，每一帧更新的行为可以放在这里
	virtual void Update();

	// 设置摄像机
	void SetCamera(Camera* camera) { m_camera = camera; }
	// 设置鼠标左右上下灵敏度
	void SetSensitivity(float s) { m_sensitivity = s; }

protected:
	// 鼠标左键是否按下
	bool m_leftMouseDown = false;
	// 鼠标右键是否按下
	bool m_rightMouseDown = false;
	// 鼠标中键是否按下
	bool m_middleMouseDown = false;
	// 当前鼠标的位置
	float m_currentX = 0.0f;
	float m_currentY = 0.0f;
	// 鼠标左右上下灵敏度
	float m_sensitivity = 0.2f;
	// 相机缩放的速度
	float m_scaleSpeed = 0.2f;
	// 记录键盘相关按键的按下状态
	std::map<unsigned, bool> m_keyMapState;
	// 摄像机
	Camera* m_camera = nullptr;
};