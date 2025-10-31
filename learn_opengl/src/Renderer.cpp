#include "../inc/Renderer.h"
#include "../inc/phongMaterial.h"
#include "../inc/OpacityMaskMaterial.h"

#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <format>
#include <algorithm>

Renderer::Renderer() 
{
	m_phongShader = std::make_shared<Shader>("assets/shaders/phong.vert", "assets/shaders/phong.frag");
	m_whiteShader = std::make_shared<Shader>("assets/shaders/white.vert", "assets/shaders/white.frag");
	m_depthShader = std::make_shared<Shader>("assets/shaders/depth.vert", "assets/shaders/depth.frag");
	m_opacityMaskShader = std::make_shared<Shader>("assets/shaders/phongOpacityMask.vert", "assets/shaders/phongOpacityMask.frag");
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

	// 默认关闭颜色混合
	glDisable(GL_BLEND);

	// 默认开启背面剔除
	glEnable(GL_CULL_FACE);
	// 默认逆时针为正面
	glFrontFace(GL_CCW);
	// 默认剔除背面
	glCullFace(GL_BACK);

	// 清理画布 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// 清空两个队列
	m_opacityObjects.clear();
	m_transparentObjects.clear();

	// 将scene中的所有mesh放入队列
	ProjectObject(scene);
	// 先绘制不透明物体，透明物体按照距离摄像机远近排序，由远到近绘制
	std::sort(
		m_transparentObjects.begin(),
		m_transparentObjects.end(),
		[camera](const Mesh* a, const Mesh* b) {
			auto viewMatrix = camera->GetViewMatrix();

			// 计算a的相机系的Z
			auto modelMatrixA = a->GetModelMatrix();
			auto worldPositionA = modelMatrixA * glm::vec4(0.0, 0.0, 0.0, 1.0);
			auto cameraPositionA = viewMatrix * worldPositionA;

			// 计算b的相机系的Z
			auto modelMatrixB = b->GetModelMatrix();
			auto worldPositionB = modelMatrixB * glm::vec4(0.0, 0.0, 0.0, 1.0);
			auto cameraPositionB = viewMatrix * worldPositionB;

			return cameraPositionA.z < cameraPositionB.z;
		}
	);

	// 先绘制不透明物体
	for (int i = 0; i < m_opacityObjects.size(); i++) 
	{
		RenderObject(m_opacityObjects[i], camera, dirLight, ambLight);
	}
	// 透明物体按照距离摄像机远近排序，由远到近绘制
	for (int i = 0; i < m_transparentObjects.size(); i++) 
	{
		RenderObject(m_transparentObjects[i], camera, dirLight, ambLight);
	}
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
	case MaterialType::OpacityMaskMaterial:
		return m_opacityMaskShader;
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
		auto geometry = mesh->m_geometry.get();
		auto material = mesh->m_material.get();

		// 考察是否拥有全局材质
		if (m_globalMaterial != nullptr) 
		{
			material = m_globalMaterial;
		}

		// 设置渲染状态
		SetDepthState(material);
		SetPolygonOffsetState(material);
		SetStencilState(material);
		SetBlendState(material);

		// 决定使用哪个Shader 
		std::shared_ptr<Shader> shader = PickShader(material->m_type);

		// 更新shader的uniform
		shader->Begin();

		switch (material->m_type) 
		{
		case MaterialType::PhongMaterial: 
		{
			PhongMaterial* phongMat = (PhongMaterial*)material;

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

			// 透明度
			shader->SetUniformFloat("opacity", material->m_opacity);

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
		case MaterialType::OpacityMaskMaterial:
		{
			OpacityMaskMaterial* opacityMat = (OpacityMaskMaterial*)material;

			// diffuse贴图帧更新
			// 将纹理采样器与纹理单元进行挂钩
			shader->SetUniformInt("sampler", opacityMat->m_diffuse->GetUnit());
			// 将纹理与纹理单元进行挂钩
			opacityMat->m_diffuse->Bind();

			// opacityMask的帧更新
			shader->SetUniformInt("opacityMaskSampler", opacityMat->m_opacityMask->GetUnit());
			opacityMat->m_opacityMask->Bind();

			// mvp
			shader->SetUniformMatrix4x4("modelMatrix", mesh->GetModelMatrix());
			shader->SetUniformMatrix4x4("viewMatrix", camera->GetViewMatrix());
			shader->SetUniformMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->GetModelMatrix())));
			shader->SetUniformMatrix3x3("normalMatrix", normalMatrix);

			// 光源参数的uniform更新
			// directionalLight 的更新
			shader->SetUniformVector3("directionalLight.color", dirLight->m_color);
			shader->SetUniformVector3("directionalLight.direction", dirLight->m_direction);
			shader->SetUniformFloat("directionalLight.specularIntensity", dirLight->m_specularIntensity);

			// shader->SetUniformFloat("shiness", opacityMat->m_shiness);

			shader->SetUniformVector3("ambientColor", ambLight->m_color);

			// 相机信息更新
			shader->SetUniformVector3("cameraPosition", camera->m_position);

			//透明度
			shader->SetUniformFloat("opacity", material->m_opacity);
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
		RenderObject(children[i], camera, dirLight, ambLight);
	}
}

void Renderer::SetDepthState(Material* material)
{
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
	else {
		glDepthMask(GL_FALSE);
	}
}

void Renderer::SetPolygonOffsetState(Material* material)
{
	if (material->m_polygonOffset) 
	{
		glEnable(material->m_polygonOffset);
		glPolygonOffset(material->m_factor, material->m_unit);
	}
	else 
	{
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_POLYGON_OFFSET_LINE);
	}
}

void Renderer::SetStencilState(Material* material)
{
	if (material->m_stencilTest) 
	{
		glEnable(GL_STENCIL_TEST);

		glStencilOp(material->m_sFail, material->m_zFail, material->m_zPass);
		glStencilMask(material->m_stencilMask);
		glStencilFunc(material->m_stencilFunc, material->m_stencilRef, material->m_stencilFuncMask);
	}
	else 
	{
		glDisable(GL_STENCIL_TEST);
	}
}

void Renderer::SetBlendState(Material* material)
{
	if (material->m_blend) 
	{
		glEnable(GL_BLEND);
		glBlendFunc(material->m_sFactor, material->m_dFactor);
	}
	else 
	{
		glDisable(GL_BLEND);
	}
}

void Renderer::ProjectObject(Object* obj)
{
	if (obj->getType() == ObjectType::Mesh) 
	{
		Mesh* mesh = (Mesh*)obj;
		auto material = mesh->m_material.get();
		if (material->m_blend) 
		{
			m_transparentObjects.push_back(mesh);
		}
		else {
			m_opacityObjects.push_back(mesh);
		}
	}

	auto children = obj->GetChildren();
	for (int i = 0; i < children.size(); i++) 
	{
		ProjectObject(children[i]);
	}
}