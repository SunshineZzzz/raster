#include "../inc/Renderer.h"
#include "../inc/phongMaterial.h"

#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

Renderer::Renderer() 
{
	m_phongShader =std::make_shared<Shader>("assets/shaders/phong.vert", "assets/shaders/phong.frag");
}

Renderer::~Renderer() {}

void Renderer::Render(
	const std::vector<std::shared_ptr<Mesh>>& meshes,
	std::shared_ptr<Camera> camera,
	std::shared_ptr<DirectionalLight> dirLight,
	std::shared_ptr<AmbientLight> ambLight
)
{
	// 设置当前帧绘制的时候，opengl的必要状态机参数
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// 清理画布 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 遍历mesh进行绘制
	for (int i = 0; i < meshes.size(); i++) 
	{
		auto mesh = meshes[i];
		auto& geometry = mesh->m_geometry;
		auto& material = mesh->m_material;

		// 决定使用哪个Shader 
		auto shader = PickShader(material->m_type);

		// 更新shader的uniform
		shader->Begin();

		switch (material->m_type) 
		{
		case MaterialType::PhongMaterial: 
		{
			PhongMaterial* phongMat = (PhongMaterial*)material.get();

			// diffuse贴图
			// 将纹理采样器与纹理单元进行挂钩
			shader->SetUniformInt("sampler", phongMat->m_diffuse->GetUnit());
			// 将纹理与纹理单元进行挂钩
			phongMat->m_diffuse->Bind();

			shader->SetUniformInt("specularMaskSampler", phongMat->m_specularMask->GetUnit());
			phongMat->m_specularMask->Bind();

			// mvp
			shader->SetUniformMatrix4x4("modelMatrix", mesh->GetModelMatrix());
			shader->SetUniformMatrix4x4("viewMatrix", camera->GetViewMatrix());
			shader->SetUniformMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());
			// 法线矩阵
			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->GetModelMatrix())));
			shader->SetUniformMatrix3x3("normalMatrix", normalMatrix);

			// 光源参数的uniform更新
			shader->SetUniformVector3("lightDirection", dirLight->m_direction);
			shader->SetUniformVector3("lightColor", dirLight->m_color);
			shader->SetUniformFloat("specularIntensity", dirLight->m_specularIntensity);
			shader->SetUniformFloat("shiness", phongMat->m_shiness);
			shader->SetUniformVector3("ambientColor", ambLight->m_color);

			// 相机信息更新
			shader->SetUniformVector3("cameraPosition", camera->m_position);
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
}

std::shared_ptr<Shader> Renderer::PickShader(MaterialType type) 
{
	switch (type) 
	{
	case MaterialType::PhongMaterial:
		return m_phongShader;
	default:
		assert(0);
	}
}
