#include "AssimpImport.h"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include <unordered_map>
#include <algorithm>

const size_t FLOATS_PER_VERTEX = 3;
const size_t VERTICES_PER_FACE = 3;

Mesh3D fromAssimpMesh(const aiMesh* mesh, const aiScene* scene, const std::filesystem::path& modelPath,
	std::unordered_map<std::filesystem::path, Texture>& loadedTextures) {

	std::vector<Vertex3D> vertices;
	// TODO: fill in this vertices list, by iterating over each element of 
	// the mVertices field of the aiMesh pointer. Each element of mVertices
	// has x, y, and z values that you can use to construct a Vertex3D object.
	// To find the u and v texture coordinates of a vertex, access the 
	// x and y fields of each element of mTextureCoords.
	// To find the normal vector components of a vertex, access the x, y, and z
	// fields of each element of mNormals.
	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		auto& meshVertex = mesh->mVertices[i];
		auto& texCoord = mesh->mTextureCoords[0][i];
		auto& normal = mesh->mNormals[i];

		// See above.

	}

	std::vector<uint32_t> faces;
	// TODO: fill in the faces list, by iterating over each element of
	// the mFaces field of the aiMesh pointer. Each element of mFaces
	// has an mIndices list, which will have three elements of its own at 
	// [0], [1], and [2]. Each of those should be pushed individually onto 
	// the faces list.
	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		auto& meshFace = mesh->mFaces[i];
		// See above.


	}

	// Load texture information from the mesh. A mesh can have more than one texture
	// associated with it; here we hard-code specific textures to load. You might
	// have to modify this if you work on a special texturing technique for your
	// project.
	std::vector<Texture> textures = {};
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// Assimp calls the base texture the "DIFFUSE" texture. We call it
		// "baseTexture" in our shaders.
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "baseTexture", modelPath, loadedTextures);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// The "SPECULAR" texture is a specular map. We call it "specMap"
		// in our shaders.
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "specMap", modelPath, loadedTextures);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		// Normal maps can either be referred to as HEIGHT or NORMALS textures in Assimp.
		// We'll load both of them as the "normalMap" shader texture.
		std::vector<Texture> normalMaps = loadMaterialTextures(material,
			aiTextureType_HEIGHT, "normalMap", modelPath, loadedTextures);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		normalMaps = loadMaterialTextures(material,
			aiTextureType_NORMALS, "normalMap", modelPath, loadedTextures);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	return Mesh3D(std::move(vertices), std::move(faces), std::move(textures));
}

/**
 * @brief Loads a list of image files into VRAM, and returns their OpenGL identifiers.
 */
std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, 
	const std::filesystem::path& modelPath,
	std::unordered_map<std::filesystem::path, Texture> &loadedTextures) {

	std::vector<Texture> textures;
	// Iterate through each file in the material's list of textures. If the file has not already
	// been loaded (into "loadedTextures"), load it into VRAM and add it to loadedTextures.
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString name;
		mat->GetTexture(type, i, &name);
		std::filesystem::path texPath = modelPath.parent_path() / name.C_Str();

		auto existing = loadedTextures.find(texPath);
		if (existing != loadedTextures.end()) {
			// The texture has already been loaded.
			textures.push_back(existing->second);
		}
		else {
			// The texture has not been loaded. Read it into RAM, then VRAM.
			sf::Image image;
			image.loadFromFile(texPath.string());
			Texture tex = Texture::loadImage(image, typeName);
			textures.push_back(tex);
			loadedTextures.insert(std::make_pair(texPath, tex));
		}
	}
	return textures;
}

/**
 * @brief Loads an hierarchical model from a single Assimp asset file, into an
 * Object3D object with 0 or more hierarchical children.
 * @param flipTextureCoords true if the origin of the model's texture space
 * is the lower-left corner of the texture, instead of the upper-left. 
 * (If your model looks wrong, try switching this value.)
 * @return 
 */
Object3D assimpLoad(const std::string& path, bool flipTextureCoords) {
	Assimp::Importer importer;

	auto options = aiProcessPreset_TargetRealtime_MaxQuality;
	if (flipTextureCoords) {
		options |= aiProcess_FlipUVs;
	}
	const aiScene* scene = importer.ReadFile(path, options);

	// If the import failed, report it
	if (nullptr == scene) {
		throw std::runtime_error("Error loading assimp file ");
	}
	std::unordered_map<std::filesystem::path, Texture> loadedTextures;
	auto ret = processAssimpNode(scene->mRootNode, scene, std::filesystem::path(path), loadedTextures);

	// aiNode -> Object3D. the aiNode's mTransformation -> Object3D.m_baseTransform.
	// The list of meshes in aiNode -> Model3D.
	return ret;
}

/**
 * @brief Loads the contents of an aiNode into one or more meshes managed by an Object3D.
 * @param node the node from the scene to process.
 * @param scene the whole scene.
 * @param modelPath the path of where the model file was loaded from.
 * @param loadedTextures all textures that have been already loaded into OpenGL.
 * @return An Object3D that contains the node's meshes and is positioned where the node
 * is supposed to be in the scene.
 */
Object3D processAssimpNode(aiNode* node, const aiScene* scene,
	const std::filesystem::path& modelPath, 
	std::unordered_map<std::filesystem::path, Texture>& loadedTextures) {
	
	// Load the aiNode's meshes.
	std::vector<Mesh3D> meshes;
	for (auto i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(fromAssimpMesh(mesh, scene, modelPath, loadedTextures));
	}

	// Make a copy of the texture identifiers loaded so far.
	std::vector<Texture> textures;
	for (auto& p : loadedTextures) {
		textures.push_back(p.second);
	}

	glm::mat4 baseTransform{ 0 };
	// Each aiNode has a base transformation that should be applied to move the node's mesh
	// to the correct position relative to its parent, BEFORE applying further transformations.
	// But this matrix is stored in assimp as a row-major matrix. GLM uses column-major matrices,
	// so we have to copy the transpose of the assimp matrix to a GLM matrix.
	for (auto i = 0; i < 4; i++) {
		for (auto j = 0; j < 4; j++) {
			baseTransform[i][j] = node->mTransformation[j][i];
		}
	}

	auto parent = Object3D(std::move(meshes), baseTransform);
	parent.setName(node->mName.C_Str());

	// Recursively process the node's children, and add them as child Object3D values.
	for (auto i = 0; i < node->mNumChildren; i++) {
		Object3D child = processAssimpNode(node->mChildren[i], scene, modelPath, loadedTextures);
		parent.addChild(std::move(child));
	}

	return parent;
}
