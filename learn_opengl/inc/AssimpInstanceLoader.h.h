#pragma once


#include "Object.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "InstancedMesh.h"
#include "Texture.h"

class AssimpInstanceLoader
{
public:
	// 根据路径加载模型
	static Object* Load(const std::string& path, int instanceCount);

private:
	// 处理节点
	static void ProcessNode(aiNode* ainode,
		Object* parent,
		const aiScene* scene,
		const std::string& rootPath,
		int instanceCount
	);
	// 处理mesh
	static InstancedMesh* ProcessMesh(
		aiMesh* aimesh,
		const aiScene* scene,
		const std::string& rootPath,
		int instanceCount
	);
	// 处理纹理
	static Texture* ProcessTexture(
		const aiMaterial* aimat,
		const aiTextureType& type,
		const aiScene* scene,
		const std::string& rootPath
	);
	// 将Assimp库使用的4X4矩阵类型转换成GLM库使用的4X4矩阵类型
	static glm::mat4 GetMat4f(aiMatrix4x4 value);
};