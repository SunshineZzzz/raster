#pragma once

#include <vector>

#include "Mesh.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "Shader.h"
#include "PointLight.h"
#include "SpotLight.h"

class Renderer 
{
public:
	Renderer();
	~Renderer();

	// 每次调用都会渲染一帧
	void Render(
		const std::vector<std::shared_ptr<Mesh>>& meshes,
		std::shared_ptr<Camera> camera,
		std::shared_ptr<DirectionalLight> dirLight,
		std::shared_ptr<PointLight> pointLight,
		std::shared_ptr<SpotLight> spotLight,
		std::shared_ptr<AmbientLight> ambLight
	);

private:
	// 根据Material类型不同，挑选不同的shader
	std::shared_ptr<Shader> PickShader(MaterialType type);

private:
	// 冯氏光照模型材质对应的shader
	std::shared_ptr<Shader> m_phongShader{ nullptr };
	// 点光源材质对应的shader
	std::shared_ptr<Shader> m_whiteShader{ nullptr };
};