#include "../inc/AssimpInstanceLoader.h.h"
#include "../inc/Tools.h"
#include "../inc/PhongMaterial.h"
#include "../inc/PhongInstanceMaterial.h"
#include "../inc/InstancedMesh.h"

Object* AssimpInstanceLoader::Load(const std::string& path, int instanceCount)
{
	// 拿出模型所在目录
	std::size_t lastIndex = path.find_last_of("//");
	auto rootPath = path.substr(0, lastIndex + 1);

	Object* rootNode = new Object();

	Assimp::Importer importer;
	// aiProcess_Triangulate 三角形化
	// aiProcess_GenNormals 生成法线
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

	// 验证读取是否正确顺利
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
	{
		return nullptr;
	}

	ProcessNode(scene->mRootNode, rootNode, scene, rootPath, instanceCount);

	return rootNode;
}

void AssimpInstanceLoader::ProcessNode(aiNode* ainode, Object* parent, const aiScene* scene, 
	const std::string& rootPath, int instanceCount)
{
	Object* node = new Object();
	parent->AddChild(node);

	// 获取
	glm::mat4 localMatrix = GetMat4f(ainode->mTransformation);
	// 位置 旋转 缩放
	glm::vec3 position, eulerAngle, scale;
	Tools::Decompose(localMatrix, position, eulerAngle, scale);
	node->SetPosition(position);
	node->SetAngleX(eulerAngle.x);
	node->SetAngleY(eulerAngle.y);
	node->SetAngleZ(eulerAngle.z);
	node->SetScale(scale);

	// 检查有没有mesh，并且解析
	for (unsigned int i = 0; i < ainode->mNumMeshes; i++) 
	{
		int meshID = ainode->mMeshes[i];
		aiMesh* aimesh = scene->mMeshes[meshID];
		auto mesh = ProcessMesh(aimesh, scene, rootPath, instanceCount);
		node->AddChild(mesh);
	}

	// 递归解析孩子
	for (unsigned int i = 0; i < ainode->mNumChildren; i++)
	{
		ProcessNode(ainode->mChildren[i], node, scene, rootPath, instanceCount);
	}
}

InstancedMesh* AssimpInstanceLoader::ProcessMesh(aiMesh* aimesh, const aiScene* scene, 
	const std::string& rootPath, int instanceCount)
{
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> uvs;
	std::vector<unsigned int> indices;
	std::vector<float> colors;

	for (unsigned int i = 0; i < aimesh->mNumVertices; i++)
	{
		// 第i个顶点的位置
		positions.push_back(aimesh->mVertices[i].x);
		positions.push_back(aimesh->mVertices[i].y);
		positions.push_back(aimesh->mVertices[i].z);

		// 第i个顶点的法线
		normals.push_back(aimesh->mNormals[i].x);
		normals.push_back(aimesh->mNormals[i].y);
		normals.push_back(aimesh->mNormals[i].z);

		//第i个顶点的颜色
		if (aimesh->HasVertexColors(0)) 
		{
			colors.push_back(aimesh->mColors[0][i].r);
			colors.push_back(aimesh->mColors[0][i].g);
			colors.push_back(aimesh->mColors[0][i].b);
		}

		// 第i个顶点的uv
		// 有可能有多套uv，关注其第0套uv，一般情况下是贴图uv
		if (aimesh->mTextureCoords[0]) 
		{
			uvs.push_back(aimesh->mTextureCoords[0][i].x);
			uvs.push_back(aimesh->mTextureCoords[0][i].y);
		}
		else 
		{
			uvs.push_back(0.0f);
			uvs.push_back(0.0f);
		}
	}

	// 解析mesh中的索引值
	for (unsigned int i = 0; i < aimesh->mNumFaces; i++)
	{
		aiFace face = aimesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	auto geometry = new Geometry(positions, normals, uvs, colors, indices);
	auto material = new PhongInstanceMaterial();

	if (aimesh->mMaterialIndex >= 0) 
	{
		Texture* texture = nullptr;
		aiMaterial* aiMat = scene->mMaterials[aimesh->mMaterialIndex];
		texture = ProcessTexture(aiMat, aiTextureType_DIFFUSE, scene, rootPath);
		if (texture == nullptr) 
		{
			texture = Texture::CreateTexture("assets/textures/defaultTexture.jpg", 0);
		}
		material->m_diffuse = texture;
	}
	else 
	{
		material->m_diffuse = Texture::CreateTexture("assets/textures/defaultTexture.jpg", 0);
	}

	return new InstancedMesh(geometry, material, instanceCount);
}


Texture* AssimpInstanceLoader::ProcessTexture(
	const aiMaterial* aimat,
	const aiTextureType& type,
	const aiScene* scene,
	const std::string& rootPath
) {
	Texture* texture = nullptr;
	// 获取图片读取路径
	aiString aipath;
	// 对应type的纹理中第0个纹理路径
	aimat->Get(AI_MATKEY_TEXTURE(type, 0), aipath);
	if (!aipath.length) 
	{
		return nullptr;
	}

	// 判断是否是嵌入fbx的图片
	const aiTexture* aitexture = scene->GetEmbeddedTexture(aipath.C_Str());
	if (aitexture) 
	{
		// 纹理图片是内嵌的
		unsigned char* dataIn = reinterpret_cast<unsigned char*>(aitexture->pcData);
		// 通常情况下（png、jpg），代表了整张图片大小
		uint32_t widthIn = aitexture->mWidth;
		uint32_t heightIn = aitexture->mHeight;
		texture = Texture::CreateTextureFromMemory(aipath.C_Str(), 0, dataIn, widthIn, heightIn);
	}
	else 
	{
		// 纹理图片在硬盘上
		std::string fullPath = rootPath + aipath.C_Str();
		texture = Texture::CreateTexture(fullPath, 0);
	}

	return texture;
}

glm::mat4 AssimpInstanceLoader::GetMat4f(aiMatrix4x4 value)
{
	glm::mat4 to(
		value.a1, value.a2, value.a3, value.a4,
		value.b1, value.b2, value.b3, value.b4,
		value.c1, value.c2, value.c3, value.c4,
		value.d1, value.d2, value.d3, value.d4
	);

	return to;
}
