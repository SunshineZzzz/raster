#pragma once

#include "Object.h"

class Scene : public Object 
{
public:
	Scene() 
	{
		m_type = ObjectType::Scene;
	};
	~Scene() {};
};