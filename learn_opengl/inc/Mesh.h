#pragma once

#include "Object.h"
#include "Geometry.h"
#include "Material.h"

#include <memory>

class Mesh : public Object 
{
public:
	Mesh(Geometry* geometry, Material* material)
	{
		m_geometry.reset(geometry);
		m_material.reset(material);
		m_type = ObjectType::Mesh;
	}
	~Mesh() {}

public:
	// 几何体
	std::shared_ptr<Geometry> m_geometry{ nullptr };
	// 材质(纹理+漫反射+镜面反射+环境光)
	std::shared_ptr<Material> m_material{ nullptr };
};