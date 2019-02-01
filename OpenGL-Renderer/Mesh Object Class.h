#pragma once
struct Material_Specs {
	vec3 Ambient_Factor;
	vec3 Diffuse_Factor;
	vec3 Specular_Factor;
	float Shininess;
};

float proccess_Timer = 0.0f;
float lastloop_Time = 0.0f;
float currentloop_Time = 0.0f;

class Mesh_Object {
private:
	vector<Mesh> Model;
	Material_Specs Material_Inputs;
	vector<Light_Object*> Lights;
	unsigned int Point_Light_Number = 1;
	unsigned int Directional_Light_Number = 0;
	unsigned int Spot_Light_Number = 0;
	

	mat4 world_transform;
	bool is_Set_World_Transform = false;
	float TextCoord_Sampler = 1;
public:
	//Empty Initializators
	Mesh_Object(const vector<Mesh> Meshes) : Model(Meshes) {}
	Mesh_Object(const vector<Mesh> Meshes, const mat4 &world_transform, const vec3 &Light_Color) : Model(Meshes), world_transform(world_transform), is_Set_World_Transform(true) {}
	Mesh_Object(const vector<Mesh> Meshes, const mat4 &world_transform) : Model(Meshes), world_transform(world_transform), is_Set_World_Transform(true) {}

	void add_Light(Light_Object *light) {
		Lights.push_back(light);
	}

	void set_Light_Numbers(unsigned int point_lights, unsigned int directional_lights, unsigned int spot_lights) {
		Point_Light_Number = point_lights;
		Directional_Light_Number = directional_lights;
		Spot_Light_Number = spot_lights;
	}
	void set_ViewMatrix(const mat4 &i_view_matrix) {
		for (Mesh &current_Mesh : Model) {
			current_Mesh.set_ViewMatrix(i_view_matrix);
		}
	}
	void set_ProjMatrix(mat4 i_proj_matrix) {
		for (Mesh &current_Mesh : Model) {
			current_Mesh.set_ProjMatrix(i_proj_matrix);
		}
	}
	void set_Shader(ShaderProgram &shader) {
		Mesh &first_Mesh = Model[0];
		if (first_Mesh.ShaderProgramID == shader.ShaderProgramID) {
			cout << "Shader is already active on Mesh\n";
			return;
		}
		for (Mesh &current_Mesh : Model) {
			current_Mesh.setShader(shader);
			cout << "Shader is active\n\n\n\n";
		}
	}
	
	void Draw() {
		for (Mesh &current_Mesh : Model) {
			//Set the Parameters We Need
			glUseProgram(current_Mesh.Shader.ShaderProgramID);
			glUniformMatrix4fv(current_Mesh.Shader.GetUniform("world_transform"), 1, GL_FALSE, value_ptr(world_transform));

			
			//Set Light Numbers
			glUniform1ui(current_Mesh.Shader.GetUniform("point_light_number"), 3);
			glUniform1ui(current_Mesh.Shader.GetUniform("directional_light_number"), Directional_Light_Number);
			glUniform1ui(current_Mesh.Shader.GetUniform("spot_light_number"), Spot_Light_Number);

			
			//Set Material Specs
			glUniform3f(current_Mesh.Shader.GetUniform("material.Ambient_Factor"), Material_Inputs.Ambient_Factor.x, Material_Inputs.Ambient_Factor.y, Material_Inputs.Ambient_Factor.z);
			glUniform3f(current_Mesh.Shader.GetUniform("material.Diffuse_Factor"), Material_Inputs.Diffuse_Factor.x, Material_Inputs.Diffuse_Factor.y, Material_Inputs.Diffuse_Factor.z);
			glUniform3f(current_Mesh.Shader.GetUniform("material.Specular_Factor"), Material_Inputs.Specular_Factor.x, Material_Inputs.Specular_Factor.y, Material_Inputs.Specular_Factor.z);
			glUniform1f(current_Mesh.Shader.GetUniform("material.Shininess"), Material_Inputs.Shininess);

			//Set Frustum Parameters to Linearize the Depth
			glUniform1f(current_Mesh.Shader.GetUniform("frustum_near"), frustum_near);
			glUniform1f(current_Mesh.Shader.GetUniform("frustum_far"), frustum_far);

			
			//Set per Object Parameters
			glUniform1f(current_Mesh.Shader.GetUniform("mixture_level"), mixture);
			glUniform3f(current_Mesh.Shader.GetUniform("view_Pos"), main_Camera->Position.x, main_Camera->Position.y, main_Camera->Position.z);
			glUniform1f(current_Mesh.Shader.GetUniform("TextCoordSampler"), TextCoord_Sampler);
			current_Mesh.Draw();
		}
	}

	void set_Material_Specs(const Material_Specs &i_material) {
		Material_Inputs.Ambient_Factor = i_material.Ambient_Factor;
		Material_Inputs.Diffuse_Factor = i_material.Diffuse_Factor;
		Material_Inputs.Specular_Factor = i_material.Specular_Factor;
		Material_Inputs.Shininess = i_material.Shininess;
	}

	void set_TextCoord_Sampler(const float &sampler) {
		TextCoord_Sampler = sampler;
	}

	void set_World_Transform(const mat4 &transform) {
		world_transform = transform;
	}
};


Mesh_Object Import_Mesh(const Import &importer, ShaderProgram Shader) {
	vector<Mesh> Meshes;
	for (Mesh mesh : importer.allMeshes) {
		mesh.setShader(Shader);
		Meshes.push_back(mesh);
	}
	return Mesh_Object(Meshes);
}