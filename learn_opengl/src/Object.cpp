#include "../inc/Object.h"

#include <glm/gtc/matrix_transform.hpp>

Object::Object() 
{
	m_type = ObjectType::Object;
}

Object::~Object() {}

void Object::SetPosition(glm::vec3 pos) 
{
	m_position = pos;
}

glm::vec3 Object::GetPosition()
{
	return m_position;
}

void Object::RotateX(float angle) 
{
	m_angleX += angle;
}

void Object::RotateY(float angle) 
{
	m_angleY += angle;
}

void Object::RotateZ(float angle) 
{
	m_angleZ += angle;
}

void Object::SetScale(glm::vec3 scale) 
{
	m_scale = scale;
}

glm::mat4 Object::GetModelMatrix() const
{
	//首先获取父亲的变换矩阵
	glm::mat4 parentMatrix{ 1.0f };
	if (m_parent != nullptr) 
	{
		parentMatrix = m_parent->GetModelMatrix();
	}

	// unity标准，缩放，旋转，平移
	glm::mat4 transform{ 1.0f };

	transform = glm::scale(transform, m_scale);
	transform = glm::rotate(transform, glm::radians(m_angleX), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(m_angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(m_angleZ), glm::vec3(0.0f, 0.0f, 1.0f));

	transform = parentMatrix * glm::translate(glm::mat4(1.0f), m_position) * transform;

	return transform;
}

void Object::AddChild(Object* obj)
{
	auto iter = std::find(m_children.begin(), m_children.end(), obj);
	if (iter != m_children.end()) 
	{
		assert(0);
	}

	m_children.push_back(obj);
	obj->m_parent = this;
}

std::vector<Object*> Object::GetChildren()
{
	return m_children;
}

Object* Object::GetParent()
{
	return m_parent;
}