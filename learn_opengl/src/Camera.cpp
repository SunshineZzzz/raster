#include <glm/gtc/matrix_transform.hpp>

#include "../inc/Camera.h"

Camera::Camera() {}

Camera::~Camera() {}

glm::mat4 Camera::GetViewMatrix() {
	// 看向-Z轴
	glm::vec3 front = glm::cross(m_up, m_right);
	glm::vec3 center = m_position + front;
	// lookat 
	// eye: 相机位置
	// center：看向世界坐标的哪个点
	// top：穹顶
	return glm::lookAt(m_position, center, m_up);
}
