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
	m_depthShader = std::make_shared<Shader>("assets/shaders/depth.vert", "assets/shaders/depth.frag");
}

Renderer::~Renderer() {}

void Renderer::Render(
	Scene* scene,
	std::shared_ptr<Camera> camera,
	std::shared_ptr<DirectionalLight> dirLight,
	std::shared_ptr<AmbientLight> ambLight
)
{
	// 设置当前帧绘制的时候，opengl的必要状态机参数
	// 有可能绘制最后一个mesh的时候，状态机参数被修改了，所以需要重新设置
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	// 关闭多边形面/线偏移
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_POLYGON_OFFSET_LINE);

	// 开启测试、设置基本写入状态，打开模板测试写入
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0xFF);

	// 清理画布 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
	case MaterialType::DepthMaterial:
		return m_depthShader;
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

		// 是否开启深度写入
		if (material->m_depthWrite) 
		{
			glDepthMask(GL_TRUE);
		}
		else 
		{
			glDepthMask(GL_FALSE);
		}

		// 是否检测使用polygonOffset
		if (material->m_polygonOffset) 
		{
			glEnable(material->m_polygonOffsetType);
			glPolygonOffset(material->m_factor, material->m_unit);
		}
		else 
		{
			glDisable(GL_POLYGON_OFFSET_FILL);
			glDisable(GL_POLYGON_OFFSET_LINE);
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
		case MaterialType::DepthMaterial:
		{
			// mvp
			shader->SetUniformMatrix4x4("modelMatrix", mesh->GetModelMatrix());
			shader->SetUniformMatrix4x4("viewMatrix", camera->GetViewMatrix());
			shader->SetUniformMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());
			// 相机信息更新
			shader->SetUniformFloat("near", camera->m_near);
			shader->SetUniformFloat("far", camera->m_far);
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
		if (i == 0) 
		{
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xFF);
		}
		else if (i == 1) 
		{
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glStencilMask(0x00);
		}
		else if (i == 2) 
		{
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xFF);
		}
		else if (i == 3)
		{
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glStencilMask(0x00);
		}
		RenderObject(children[i], camera, dirLight, ambLight);
	}
}