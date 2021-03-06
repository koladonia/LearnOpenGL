#include "Model.h"

Model::Model(const std::string& path)
{
	glGenBuffers(1, &materialUBOId);
	glBindBuffer(GL_UNIFORM_BUFFER, materialUBOId);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialUBO), nullptr, GL_STATIC_DRAW);
	
	loadModel(path);
}

void Model::draw(bool noTex, uint32_t materialUBOBindingPoint)
{
	if (!noTex)
		glBindBufferBase(GL_UNIFORM_BUFFER, materialUBOBindingPoint, materialUBOId);
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].draw(noTex, materialUBOId);
}

void Model::loadModel(const std::string& path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Mesh::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Mesh::Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Mesh::Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		if (mesh->HasNormals())
		{
			vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}
		if (mesh->HasTextureCoords(0))
		{
			vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		if (mesh->HasTangentsAndBitangents())
		{
			vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}
		
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Mesh::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Mesh::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Mesh::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, Mesh::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		std::vector<Mesh::Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, Mesh::NORMAL);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		std::vector<Mesh::Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, Mesh::HEIGHT);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Mesh::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, Mesh::TexType texType)
{
	std::vector<Mesh::Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString path;
		mat->GetTexture(type, i, &path);
		std::string pathStr(path.C_Str());
		bool skip = false;
		for (unsigned int j = 0; j < texturesLoaded.size() && !skip; j++)
		{
			if (texturesLoaded[j].path == pathStr)
			{
				textures.push_back(texturesLoaded[j]);
				skip = true;
			}
		}
		if (!skip)
		{
			bool gamma = false;
			if (texType == Mesh::DIFFUSE)
				gamma = true;
			Mesh::Texture texture = { textureFromFile(pathStr, directory, gamma), texType, pathStr };
			textures.push_back(texture);
			texturesLoaded.push_back(texture);
		}
	}
	return textures;
}

GLuint Model::textureFromFile(const std::string &path, const std::string &directory, bool gamma)
{
	std::string fullPath = directory + '/' + path;

	GLuint textureID;
	glGenTextures(1, &textureID);

	int width;
	int height;
	int nrComponents;

	unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		GLenum intFormat;
		switch (nrComponents)
		{
			case 1: 
				format = GL_RED;
			break;
			case 2:
				format = GL_RG;
			break;
			case 3:
				intFormat = gamma ? GL_SRGB : GL_RGB;
				format = GL_RGB;
				break;
			case 4:
				intFormat = gamma ? GL_SRGB_ALPHA : GL_RGBA;
				format = GL_RGBA;
				break;
			default:
				format = GL_RGB;
			break;
		}
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, intFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		float aniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, aniso);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cerr << "Texture failed to load at path: " << fullPath << std::endl;
	}
	stbi_image_free(data);
	return textureID;
}
