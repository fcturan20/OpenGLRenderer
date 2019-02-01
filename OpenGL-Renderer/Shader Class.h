#pragma once

//Read the Shader Code File and Convert it to a String
string ReadShader(const string &shaderPath_string) {
	const char* shaderPath = shaderPath_string.c_str();
	ifstream shaderFile;
	shaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{
		shaderFile.open(shaderPath);
		stringstream shaderFile_stream;
		shaderFile_stream << shaderFile.rdbuf();
		shaderFile.close();
		return shaderFile_stream.str();
	}
	catch (ifstream::failure error)
	{
		cout << "Error: Shader file couldn't read: " << shaderPath_string << endl;
		return "Error";
	}
}


vector<GLuint> ShaderIDs;

/*Instructions:
1) This class is for you to create a Shader Program as you wish
2) You can create a Shader Program with;
a. Main Source Code of Vertex and Fragment Shaders
b. One Shader Object and One Source Code
c. Linking 2 Compiled Shader Objects
3) You can access any Shader whenever and wherever you want
4) This class can't handle with destroying/allocating Shaders because I couldn't be succesfull on it. Help would mean a lot to me.



*/
class ShaderProgram {
public:
	GLuint VertexShaderID;
	GLuint FragmentShaderID;
	GLuint ShaderProgramID;

	GLuint compile_VertexShader(const char* VertexShader);
	GLuint compile_FragmentShader(const char* FragmentShader);
	GLuint linkShaders_toProgram(GLuint VertexShader_o, GLuint FragmentShader_o);
	GLuint compile_All(const char* VertexShader_s, const char *FragmentShader_s);
	const GLint GetUniform(const char* variable_name);
	void set_Texture(const GLuint &texture_slot, const char* variable_name, const GLuint &texture_id) {
		glActiveTexture(GL_TEXTURE0 + texture_slot);
		glUniform1i(glGetUniformLocation(this->ShaderProgramID, variable_name), texture_slot);
		glBindTexture(GL_TEXTURE_2D, texture_id);
	}

	ShaderProgram() {
		compile_All(ReadShader("Default.vert").c_str(), ReadShader("Default.frag").c_str());
		cout << "ShaderProgramID: " << ShaderProgramID << endl;
		ShaderIDs.push_back(ShaderProgramID);
	}

	ShaderProgram(const string &VertexShader_path, const string &FragmentShader_path) {
		compile_All(ReadShader(VertexShader_path).c_str(), ReadShader(FragmentShader_path).c_str());
		cout << "ShaderProgramID: " << ShaderProgramID << endl;
		ShaderIDs.push_back(ShaderProgramID);
	}

	ShaderProgram(const string &VertexShader_s, GLuint FragmentShader_o) : FragmentShaderID(FragmentShader_o) {
		compile_VertexShader(ReadShader(VertexShader_s).c_str());
		linkShaders_toProgram(VertexShaderID, FragmentShaderID);
		cout << "ShaderProgramID: " << ShaderProgramID << endl;
		ShaderIDs.push_back(ShaderProgramID);
	}

	ShaderProgram(GLuint VertexShader_o, const string &FragmentShader_s) : VertexShaderID(VertexShader_o) {
		compile_FragmentShader(ReadShader(FragmentShader_s).c_str());
		linkShaders_toProgram(VertexShaderID, FragmentShaderID);
		cout << "ShaderProgramID: " << ShaderProgramID << endl;
		ShaderIDs.push_back(ShaderProgramID);
	}

	ShaderProgram(GLuint VertexShader_o, GLuint FragmentShader_o) : VertexShaderID(VertexShader_o), FragmentShaderID(FragmentShader_o) {
		linkShaders_toProgram(VertexShaderID, FragmentShaderID);
		cout << "ShaderProgramID: " << ShaderProgramID << endl;
		ShaderIDs.push_back(ShaderProgramID);
	}


	void Bind_UBOLights();
};

const GLint ShaderProgram::GetUniform(const char* variable_name) {
	return glGetUniformLocation(this->ShaderProgramID, variable_name);
}


GLuint ShaderProgram::compile_VertexShader(const char* VertexShader) {
	//Vertex Shader'ý tanýmla ve Kod Kaynaðýný Toparla (Compile et)
	GLuint vertexShader_o = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader_o, 1, &VertexShader, NULL);
	glCompileShader(vertexShader_o);

	//Toparlama sýrasýnda bir sorun oluþursa uyar
	int success;
	char infolog[512];
	glGetShaderiv(vertexShader_o, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader_o, 512, NULL, infolog);
		cout << VertexShader << endl << "Vertex Shader couldn't compile!\n" << infolog << endl;
	}

	VertexShaderID = vertexShader_o;
	return vertexShader_o;
}


GLuint ShaderProgram::compile_FragmentShader(const char* FragmentShader) {

	//Fragment Shader'ý tanýmla ve Kod Kaynaðýný Toparla (Compile et)
	GLuint fragmentShader_o = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_o, 1, &FragmentShader, NULL);
	glCompileShader(fragmentShader_o);

	//Toparlama sýrasýnda bir sorun oluþursa uyar
	int success;
	char infolog[512];
	glGetShaderiv(fragmentShader_o, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader_o, 512, NULL, infolog);
		cout << "Fragment Shader couldn't compile!\n" << infolog << endl;
	}

	FragmentShaderID = fragmentShader_o;
	return fragmentShader_o;
}


GLuint ShaderProgram::linkShaders_toProgram(GLuint vertexShader_o, GLuint fragmentShader_o) {
	//Shader Program'ýný Vertex ve Fragment Shader'larýna baðla
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader_o);
	glAttachShader(shaderProgram, fragmentShader_o);
	glLinkProgram(shaderProgram);

	//Baðlama sýrasýnda bir sorun oluþursa uyar
	int success;
	char infolog[512];
	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		cout << "Shader Program couldn't link!\n" << infolog << endl;
	}

	ShaderProgramID = shaderProgram;
	return shaderProgram;
}

GLuint ShaderProgram::compile_All(const char* VertexShader_s, const char* FragmentShader_s) {
	compile_VertexShader(VertexShader_s);
	compile_FragmentShader(FragmentShader_s);
	linkShaders_toProgram(VertexShaderID, FragmentShaderID);
	return ShaderProgramID;
}

void ShaderProgram::Bind_UBOLights() {
	unsigned int point_lights_index = glGetUniformBlockIndex(ShaderProgramID, "Lights_Point");
	/*
	unsigned int directional_lights_index = glGetUniformBlockIndex(ShaderProgramID, "Lights_Directional");
	unsigned int spot_lights_index = glGetUniformBlockIndex(ShaderProgramID, "Lights_Spot");*/


	glUniformBlockBinding(ShaderProgramID, point_lights_index, 1);
	/*
	glUniformBlockBinding(ShaderProgramID, directional_lights_index, 2);
	glUniformBlockBinding(ShaderProgramID, spot_lights_index, 3);*/
}
