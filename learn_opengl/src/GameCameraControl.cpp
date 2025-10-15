#include <SDL3/SDL.h>

#include <glm/gtc/matrix_transform.hpp>

#include "../inc/GameCameraControl.h"

GameCameraControl::GameCameraControl() {}

GameCameraControl::~GameCameraControl() {}

void GameCameraControl::OnCursor(float xPos, float yPos) 
{
	float deltaX = (xPos - m_currentX) * m_sensitivity;
	float deltaY = (yPos - m_currentY) * m_sensitivity;

	if (m_rightMouseDown) 
	{
		pitch(-deltaY);
		yaw(-deltaX);
	}

	m_currentX = xPos;
	m_currentY = yPos;
}

void GameCameraControl::pitch(float angle) 
{
	m_pitch += angle;
	if (m_pitch > 89.0f || m_pitch < -89.0f) 
	{
		m_pitch -= angle;
		return;
	}

	// 在GameCameraControl的情况下，pitch不会影响m_position
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), m_camera->m_right);
	m_camera->m_up = mat * glm::vec4(m_camera->m_up, 0.0f);

	// 这个改变完以后，视图变化矩阵会发生改变，会重新计算front(-Z轴，观察方向)，center(看向世界坐标的哪个点)
}

void GameCameraControl::yaw(float angle)
{
	// 可以想象低着头摇摆脑袋
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	m_camera->m_up = mat * glm::vec4(m_camera->m_up, 0.0f);
	m_camera->m_right = mat * glm::vec4(m_camera->m_right, 0.0f);

	// 不要这样，原因是：
	// 当你选择绕着相机自己的局部UP轴进行Yaw时(左右摇摆)，如果相机已经抬头或低头(Pitch)，就会引入歪着头(Roll)，
	// 画面发生倾斜，会破坏视觉稳定性和直觉，除非是做太空游戏或者飞行模拟。
	// auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), m_camera->m_up);
	// m_camera->m_right = mat * glm::vec4(m_camera->m_right, 0.0f);
}


void GameCameraControl::Update() 
{
	// 最终移动方向
	glm::vec3 direction(0.0f);

	// 摄像机观察方向，1长度单位
	auto front = glm::cross(m_camera->m_up, m_camera->m_right);
	// 摄像机+X轴
	auto right = m_camera->m_right;
	// SDL_Log("front:%f, right:%f\n", glm::length(front), glm::length(right));

	if (m_keyMapState[SDL_SCANCODE_W])
	{
		direction += front;
	}

	if (m_keyMapState[SDL_SCANCODE_S])
	{
		// 假设w和s都按了，此时direaction长度为1，right长度为1，相减后，长度是0，最后需要做归一化
		direction -= front;
	}

	if (m_keyMapState[SDL_SCANCODE_A])
	{
		// 假设w和a都按了，此时direaction长度为1，right长度为1，相减后，长度是根号2，最后需要做归一化
		direction -= right;
	}

	if (m_keyMapState[SDL_SCANCODE_D])
	{
		direction += right;
	}

	// 此时direction有可能不为1的长度，也有可能是0的长度
	if (glm::length(direction) != 0) 
	{
		direction = glm::normalize(direction);
		m_camera->m_position += direction * m_speed;
	}
}