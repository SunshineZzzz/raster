#pragma once

#include <glm/glm.hpp>

class Tools 
{
public:
	// 将一个4X4的模型变换矩阵分解成其组成部分：平移位置、旋转欧拉角和缩放比例
	static void Decompose(glm::mat4 matrix, glm::vec3& position, glm::vec3& eulerAngle, glm::vec3& scale);
};