#pragma once 

#include <glm/glm.hpp>
#include <vector>

enum class ObjectType 
{
	Object,
	Mesh,
	Scene
};

class Object 
{
public:
	Object();
	virtual ~Object();

	// 设置世界坐标系位置
	void SetPosition(glm::vec3 pos);
	// 获取世界坐标系位置
	glm::vec3 GetPosition();
	// 增量绕自身X/Y/Z轴旋转
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);
	void SetAngleX(float angle)
	{
		m_angleX = angle;
	}
	void SetAngleY(float angle)
	{
		m_angleY = angle;
	}
	void SetAngleZ(float angle)
	{
		m_angleZ = angle;
	}
	// 缩放
	void SetScale(glm::vec3 scale);
	// 获取模型矩阵
	glm::mat4 GetModelMatrix() const;
	// 增加孩子
	void AddChild(Object* obj);
	// 获取孩子们
	std::vector<Object*> GetChildren();
	// 获取父亲
	Object* GetParent();
	// 获取类型
	ObjectType getType()const { return m_type; }

protected:
	// 世界坐标系位置
	glm::vec3 m_position{ 0.0f };
	// 绕自身X/Y/Z轴旋转角度
	// pitch
	float m_angleX{ 0.0f };
	// yaw
	float m_angleY{ 0.0f };
	// roll
	float m_angleZ{ 0.0f };
	// 缩放
	glm::vec3 m_scale{ 1.0f };
	// 父子
	std::vector<Object*> m_children;
	Object* m_parent{nullptr};
	// 类型记录
	ObjectType m_type;
};