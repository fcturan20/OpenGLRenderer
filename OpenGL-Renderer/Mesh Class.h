#pragma once

//Mesh Class Implemented
//If you want to Render an object, you just need to call Draw() function
//But if you want to set some Parameters to Shader of Object, you need to set them before calling the Draw() function

class Import;

struct Vertex
{
	vec3 Position;
	vec3 Normal;
	vec2 TextCoord;
	vec3 Tangent;
	vec3 Bitangent;
	vec3 Color;
};

class Mesh
{
private:
	//Bools to Set Vertex Array Attribute
	bool bool_TextCoord = true, bool_Color = true;
	bool bool_Indices = true;

	//Render Buffers
	GLuint VAO, VBO, EBO;
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	ShaderProgram Shader;
	GLuint ShaderProgramID;
	void SetupMesh();
	void SetupBaseShape();

	mat4 model_world_matrix;
	mat4 view_matrix;
	mat4 projection_matrix;


	void setVertices(vector<Vertex> &vertices);
	void setIndices(vector<unsigned int> &indices);
	void setShader(ShaderProgram i_Shader) {
		Shader = i_Shader;
		ShaderProgramID = i_Shader.ShaderProgramID;
	}
	void set_ViewMatrix(const mat4 &i_view_matrix) {
		view_matrix = i_view_matrix;
	}
	void set_ProjMatrix(mat4 i_proj_matrix) {
		projection_matrix = i_proj_matrix;
	}
	void Draw();

	Mesh(vector<Vertex> Vertex, vector<unsigned int> Indices, vector<Texture> Textures, ShaderProgram &Shader) :
		vertices(Vertex), indices(Indices), textures(Textures), Shader(Shader), ShaderProgramID(Shader.ShaderProgramID) {
		mat4 world_matrix;
		model_world_matrix = world_matrix;
		SetupMesh();
	}

	Mesh(vector<Vertex> Vertex, vector<unsigned int> Indices) : vertices(Vertex), indices(Indices) {
		mat4 world_matrix;
		model_world_matrix = world_matrix;
		SetupBaseShape();
	}

	Mesh(vector<Vertex> Vertex) : vertices(Vertex) {
		mat4 world_matrix;
		model_world_matrix = world_matrix;
		SetupBaseShape();
	}
};

void Mesh::setVertices(vector<Vertex> &vertices) {
	for (Vertex Vertex : vertices) {
		vertices.push_back(Vertex);
	}
}

void Mesh::setIndices(vector<unsigned int> &indices) {
	for (unsigned int indice_var : indices) {
		indices.push_back(indice_var);
	}
}


void Mesh::SetupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextCoord));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
	glBindVertexArray(0);
}

void Mesh::SetupBaseShape() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	if (bool_Indices) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) + sizeof(vec3), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex) + sizeof(vec3), (void*)offsetof(Vertex, TextCoord));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) + sizeof(vec3), (void*)(offsetof(Vertex, Color) + sizeof(vec3)));
	glBindVertexArray(0);
}


void Mesh::Draw() {
	glUseProgram(Shader.ShaderProgramID);
	glUniformMatrix4fv(Shader.GetUniform("world_transform"), 1, GL_FALSE, value_ptr(model_world_matrix));
	glUniformMatrix4fv(Shader.GetUniform("projection_matrix"), 1, GL_FALSE, value_ptr(global_projection_matrix));
	glUniformMatrix4fv(Shader.GetUniform("view_matrix"), 1, GL_FALSE, value_ptr(view_matrix));

	unsigned int i = 0;
	for (Texture texture : textures) {
		glActiveTexture(GL_TEXTURE0 + i);

		glUniform1i(Shader.GetUniform(texture.string_name.c_str()), i);
		glBindTexture(texture.texture_D, texture.id);
		i++;
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

