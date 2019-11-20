#pragma once

struct ImportedTextures {
	string path;
	Texture texture;
};

vector<ImportedTextures> imported_textures_vector;

class Import {
private:
	//Import Model
	string Directory;
	const aiScene *Scene;
	void importModel(string file_path);
	void proccessNode(aiNode *Node);
	Mesh createMesh(aiMesh *Mesh);
	ShaderProgram shader;
	bool TextureInput = false;
	vector<Texture> loadTextures_FromModel(aiMaterial *material, aiTextureType type, const string &type_Name);
	vector<Texture> Texture_whatset;
	bool Status = true;
	TextureProperties texture_Properties;
public:
	vector<Mesh> allMeshes;
	unsigned int MeshNumber = 0;
	unsigned int NodeNumber = 0;


	Import(string file_path, ShaderProgram Shader, vector<Texture> Textures, bool show_Status) : shader(Shader), Texture_whatset(Textures), TextureInput(true), Status(show_Status) {
		importModel(file_path);
		if (Status) {
			cout << "Mesh Number: " << MeshNumber << "\nNode Number: " << NodeNumber << endl;
		}
	}
	Import(string file_path, ShaderProgram Shader, const TextureProperties &properties, bool show_Status) : shader(Shader), Status(show_Status), texture_Properties(properties) {
		importModel(file_path);
		if (Status) {
			cout << "Mesh Number: " << MeshNumber << "\nNode Number: " << NodeNumber << endl;
		}
	}
	Import(string file_path, ShaderProgram Shader, bool show_Status) : shader(Shader), Status(show_Status) {
		importModel(file_path);
		if (Status) {
			cout << "Mesh Number: " << MeshNumber << "\nNode Number: " << NodeNumber << endl;
		}
	}
};

void Import::importModel(string file_path) {
	Assimp::Importer import;
	Scene = import.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode) {
		cout << "Failed on Loading Mesh with Assimp; " << import.GetErrorString() << endl;
		return;
	}
	Directory = file_path.substr(0, file_path.find_last_of('/'));

	proccessNode(Scene->mRootNode);
}

void Import::proccessNode(aiNode *Node) {
	for (unsigned int i = 0; i < (Node->mNumMeshes); i++) {
		aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
		allMeshes.push_back(createMesh(mesh));
		MeshNumber++;
	}
	for (unsigned int i = 0; i < Node->mNumChildren; i++) {
		NodeNumber++;
		proccessNode(Node->mChildren[i]);
	}
}

Mesh Import::createMesh(aiMesh *mesh) {
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	if (Status) {
		if (!(mesh->HasNormals())) {
			cout << "Vertex Normal'lari bulunamadi\n";
		}
		if (!(mesh->HasTextureCoords(0))) {
			cout << "Texture Koordinatlari bulunamadi\n";
		}
		if (!(mesh->HasVertexColors(0))) {
			cout << "Vertex Color bulunamadi\n";
		}
	}

	//Vertex Proccess
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		//Positions Set
		vec3 Position;
		Position.x = mesh->mVertices[i].x;
		Position.y = mesh->mVertices[i].y;
		Position.z = mesh->mVertices[i].z;
		vertex.Position = Position;
		//Normals Set
		vec3 Normal;
		Normal.x = mesh->mNormals[i].x;
		Normal.y = mesh->mNormals[i].y;
		Normal.z = mesh->mNormals[i].z;
		vertex.Normal = Normal;
		vec3 Tangent;
		Tangent.x = mesh->mTangents[i].x;
		Tangent.y = mesh->mTangents[i].y;
		Tangent.z = mesh->mTangents[i].z;
		vertex.Tangent = Tangent;

		vec3 Bitangent;
		Bitangent.x = mesh->mBitangents[i].x;
		Bitangent.y = mesh->mBitangents[i].y;
		Bitangent.z = mesh->mBitangents[i].z;
		vertex.Bitangent = Bitangent;


		if (mesh->HasTextureCoords(0)) {
			vertex.TextCoord.x = mesh->mTextureCoords[0][i].x;
			vertex.TextCoord.y = mesh->mTextureCoords[0][i].y;
		}
		else {
			vertex.TextCoord = vec2(0, 0);
		}
		if (mesh->HasVertexColors(0)) {
			vertex.Color.x = mesh->mColors[0][i].r;
			vertex.Color.y = mesh->mColors[0][i].g;
			vertex.Color.z = mesh->mColors[0][i].b;
		}
		else {
			vertex.Color = vec3(0, 0, 0);
		}
		vertices.push_back(vertex);
	}

	//Indice Proccess
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}



	if (TextureInput == false) {
		aiMaterial *material = Scene->mMaterials[mesh->mMaterialIndex];
		//Diffuse
		vector<Texture> diffuseMaps = loadTextures_FromModel(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		
		//Specular
		vector<Texture> specularMaps = loadTextures_FromModel(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		
		//Normals
		vector<Texture> normalMaps = loadTextures_FromModel(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		
		return Mesh(vertices, indices, textures, shader);
	}
	else {
		return Mesh(vertices, indices, Texture_whatset, shader);
	}
}




vector<Texture> Import::loadTextures_FromModel(aiMaterial *material, aiTextureType type, const string &type_Name) {
	vector<Texture> loadtextures{};
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
		aiString file_name;
		material->GetTexture(type, i, &file_name);

		bool is_Texture_inMemory = false;
		string texture_path = (Directory + '/' + (string)file_name.C_Str());
		
		//Check whether this Texture is loaded or not and if loaded, Bind it to the Mesh
		for (ImportedTextures imported : imported_textures_vector) {
			if (imported.path == texture_path) {
				loadtextures.push_back(imported.texture);
				is_Texture_inMemory = true;
				break;
			}
		}

		//If texture is not loaded, load it and bind to the Mesh
		if (is_Texture_inMemory == false && texture_Properties.Texture_Format != GL_RGBA) {
			//Import Texture
			Texture texture((Directory + '/' + (string)file_name.C_Str()).c_str(), (type_Name + to_string(i)), texture_Properties);
			ImportedTextures imported = {texture_path, texture};
			imported_textures_vector.push_back(imported);

			//Is this the way Best Optimized?
			unsigned int order = imported_textures_vector.size() - 1;
			const Texture &imported_texture_inMemory = imported_textures_vector[order].texture;
			loadtextures.push_back(imported_texture_inMemory);
		}


		else if(is_Texture_inMemory == false && texture_Properties.Texture_Format == GL_RGBA) {
			//Import Texture
			Texture texture((Directory + '/' + (string)file_name.C_Str()).c_str(), (type_Name + to_string(i)).c_str());
			ImportedTextures imported = { texture_path, texture };
			imported_textures_vector.push_back(imported);

			//Is this the way Best Optimized?
			unsigned int order = imported_textures_vector.size() - 1;
			const Texture &imported_texture_inMemory = imported_textures_vector[order].texture;
			loadtextures.push_back(imported_texture_inMemory);
		}
	}
	return loadtextures;
}









