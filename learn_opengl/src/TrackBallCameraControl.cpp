#include <glm/gtc/matrix_transform.hpp>

#include "../inc/TrackBallCameraControl.h"

TrackBallCameraControl::TrackBallCameraControl() {}

TrackBallCameraControl::~TrackBallCameraControl() {}

void TrackBallCameraControl::OnCursor(float xPos, float yPos)
{
	if (m_leftMouseDown) 
	{
		// 调整相机的各类参数
		// 计算经线和纬线旋转的增量角度(正负都有可能）
		float deltaX = (xPos - m_currentX) * m_sensitivity;
		float deltaY = (yPos - m_currentY) * m_sensitivity;

		// 分开pitch和yaw各自计算
		pitch(-deltaY);
		yaw(-deltaX);
	}
	else if (m_middleMouseDown) 
	{
		float deltaX = (xPos - m_currentX) * m_moveSpeed;
		float deltaY = (yPos - m_currentY) * m_moveSpeed;

		m_camera->m_position = m_camera->m_position + m_camera->m_up * deltaY;
		m_camera->m_position = m_camera->m_position - m_camera->m_right * deltaX;
	}

	m_currentX = xPos;
	m_currentY = yPos;
}

void TrackBallCameraControl::pitch(float angle) 
{
	// 绕着摄像机局部坐标系m_right向量在旋转
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), m_camera->m_right);

	// 影响当前相机的m_up向量和位置m_position 
	// 对于向量，齐次坐标是在向量的末尾添加一个分量0，只被旋转缩放剪切，而忽略平移
	m_camera->m_up = mat * glm::vec4(m_camera->m_up, 0.0f);
	// 对于点，齐次坐标是在点的末尾添加一个分量1，施加所有变化
	m_camera->m_position = mat * glm::vec4(m_camera->m_position, 1.0f);
}

void TrackBallCameraControl::yaw(float angle) 
{
	// 绕着世界坐标系的y轴旋转
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

	// 影响当前相机的m_up向量，位置m_position
	m_camera->m_up = mat * glm::vec4(m_camera->m_up, 0.0f);
	m_camera->m_right = mat * glm::vec4(m_camera->m_right, 0.0f);
	m_camera->m_position = mat * glm::vec4(m_camera->m_position, 1.0f);
}

void TrackBallCameraControl::OnScroll(float offsetY) 
{
	m_camera->Scale(m_scaleSpeed * offsetY);
}