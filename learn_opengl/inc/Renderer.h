#pragma once

#include <vector>

#include "Mesh.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "Shader.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Scene.h"

class Renderer 
{
public:
	Renderer();
	~Renderer();

	// 每次调用都会渲染一帧
	void Render(
		Scene* scene,
		std::shared_ptr<Camera> camera,
		std::shared_ptr<DirectionalLight> dirLight,
		std::shared_ptr<AmbientLight> ambLight
	);
	// 针对单个object进行渲染
	void RenderObject(
		Object* object,
		std::shared_ptr<Camera> camera,
		std::shared_ptr<DirectionalLight> dirLight,
		std::shared_ptr<AmbientLight> ambLight
	);
	// 设置背景颜色
	void SetClearColor(glm::vec3 color) 
	{
		glClearColor(color.r, color.g, color.b, 1.0);
	}
	// 设置渲染状态
	void SetDepthState(Material* material);
	void SetPolygonOffsetState(Material* material);
	void SetStencilState(Material* material);
	void SetBlendState(Material* material);

private:
	// 根据Material类型不同，挑选不同的shader
	std::shared_ptr<Shader> PickShader(MaterialType type);

private:
	// 冯氏光照模型材质对应的shader
	std::shared_ptr<Shader> m_phongShader{ nullptr };
	// 点光源材质对应的shader
	std::shared_ptr<Shader> m_whiteShader{ nullptr };
	// 深度材质对应的shader
	std::shared_ptr<Shader> m_depthShader{ nullptr };
};