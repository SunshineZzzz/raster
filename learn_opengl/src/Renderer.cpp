#include "../inc/Renderer.h"
#include "../inc/phongMaterial.h"

#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <format>

Renderer::Renderer() 
{
	m_phongShader = std::make_shared<Shader>("assets/shaders/phong.vert", "assets/shaders/phong.frag");
	m_whiteShader = std::make_shared<Shader>("assets/shaders/white.vert", "assets/shaders/white.frag");
}

Renderer::~Renderer() {}

void Renderer::Render(
	Scene* scene,
	std::shared_ptr<Camera> camera,
	std::shared_ptr<DirectionalLight> dirLight,
	std::shared_ptr<AmbientLight> ambLight
)
{
	// 清理画布 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 将scene当作根节点开始递归渲染
	RenderObject(scene, camera, dirLight, ambLight);
}

std::shared_ptr<Shader> Renderer::PickShader(MaterialType type) 
{
	switch (type) 
	{
	case MaterialType::PhongMaterial:
		return m_phongShader;
	case MaterialType::WhiteMaterial:
		return m_whiteShader;
	default:
		assert(0);
	}
	return nullptr;
}

void Renderer::RenderObject(
	Object* object,
	std::shared_ptr<Camera> camera,
	std::shared_ptr<DirectionalLight> dirLight,
	std::shared_ptr<AmbientLight> ambLight
) 
{
	// 判断是Mesh还是Object，如果是Mesh需要渲染
	if (object->getType() == ObjectType::Mesh) 
	{
		auto mesh = (Mesh*)object;
		auto& geometry = mesh->m_geometry;
		auto& material = mesh->m_material;

		// 设置渲染状态
		if (material->m_depthTest) 
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(material->m_depthFunc);
		}
		else 
		{
			glDisable(GL_DEPTH_TEST);
		}

		if (material->m_depthWrite) 
		{
			glDepthMask(GL_TRUE);
		}
		else 
		{
			glDepthMask(GL_FALSE);
		}

		// 决定使用哪个Shader 
		std::shared_ptr<Shader> shader = PickShader(material->m_type);

		// 更新shader的uniform
		shader->Begin();

		switch (material->m_type) 
		{
		case MaterialType::PhongMaterial: 
		{
			PhongMaterial* phongMat = (PhongMaterial*)material.get();

			// diffuse贴图帧更新
			// 将纹理采样器与纹理单元进行挂钩
			shader->SetUniformInt("sampler", phongMat->m_diffuse->GetUnit());
			// 将纹理与纹理单元进行挂钩
			phongMat->m_diffuse->Bind();

			// 高光蒙版的帧更新
			//shader->SetUniformInt("specularMaskSampler", phongMat->m_specularMask->GetUnit());
			//phongMat->m_specularMask->Bind();

			// mvp
			shader->SetUniformMatrix4x4("modelMatrix", mesh->GetModelMatrix());
			shader->SetUniformMatrix4x4("viewMatrix", camera->GetViewMatrix());
			shader->SetUniformMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());
			// 法线矩阵
			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->GetModelMatrix())));
			shader->SetUniformMatrix3x3("normalMatrix", normalMatrix);

			// 光源参数的uniform更新
			// directionalLight的更新
			shader->SetUniformVector3("directionalLight.color", dirLight->m_color);
			shader->SetUniformVector3("directionalLight.direction", dirLight->m_direction);
			shader->SetUniformFloat("directionalLight.specularIntensity", dirLight->m_specularIntensity);

			// 光斑大小
			shader->SetUniformFloat("shiness", phongMat->m_shiness);
			// 环境光
			shader->SetUniformVector3("ambientColor", ambLight->m_color);
			// 相机信息更新
			shader->SetUniformVector3("cameraPosition", camera->m_position);
		}
		break;
		case MaterialType::WhiteMaterial: 
		{
			// mvp
			shader->SetUniformMatrix4x4("modelMatrix", mesh->GetModelMatrix());
			shader->SetUniformMatrix4x4("viewMatrix", camera->GetViewMatrix());
			shader->SetUniformMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());
		}
		break;
		default:
			assert(0);
		}

		// 绑定vao
		glBindVertexArray(geometry->GetVao());

		// 执行绘制命令
		glDrawElements(GL_TRIANGLES, geometry->GetIndicesCount(), GL_UNSIGNED_INT, 0);
	}


	// 遍历object的子节点，对每个子节点都需要调用renderObject
	auto children = object->GetChildren();
	for (int i = 0; i < children.size(); i++) 
	{	
		//if (i == 1) 
		//{
		//	glDepthMask(GL_FALSE);
		//}
		//else 
		//{
		//	glDepthMask(GL_TRUE);
		//}
		RenderObject(children[i], camera, dirLight, ambLight);
	}
}