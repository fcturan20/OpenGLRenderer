#pragma once
struct base_Light {
	vec3 color = Sentinel_Values::normalized_Vec3;
	float constant = Sentinel_Values::attenuation_constant;
	float linear = Sentinel_Values::attenuation_linear;
	float quadratic = Sentinel_Values::attenuation_quadratic;
	unsigned int calculate_Attenuation = false;
};

struct PointLight {
	vec4 world_position;
	vec4 color = Sentinel_Values::normalized_Vec4;
	vec4 constant = (vec4)Sentinel_Values::attenuation_constant;
	vec4 linear = (vec4)Sentinel_Values::attenuation_linear;
	vec4 quadratic = (vec4)Sentinel_Values::attenuation_quadratic;
};
unsigned int sizeof_PointLight = 80;
vector<PointLight> Lights_Point;


struct DirectionalLight {
	vec4 direction;
	vec4 color = vec4(Sentinel_Values::normalized_Vec3, 1.0f);
};
unsigned int sizeof_DirectionalLight = 32;
vector<DirectionalLight> Lights_Directional;

struct SpotLight {
	vec4 world_position = vec4(Sentinel_Values::normalized_Vec3, 1.0f);
	vec4 color = vec4(Sentinel_Values::normalized_Vec3, 1.0f);
	vec4 direction = vec4(Sentinel_Values::normalized_Vec3, 1.0f);
	vec4 cos_Inner_CutOff = (vec4)Sentinel_Values::cos_Inner_CutOff;
	vec4 cos_Outer_CutOff = (vec4)Sentinel_Values::cos_Outer_CutOff;
	vec4 constant = (vec4)Sentinel_Values::attenuation_constant;
	vec4 linear = (vec4)Sentinel_Values::attenuation_linear;
	vec4 quadratic = (vec4)Sentinel_Values::attenuation_quadratic;
};
unsigned int sizeof_SpotLight = 300;
vector<SpotLight> Lights_Spot;



class Light_Object {
	vector<Mesh> Object_Model;
	vec3 Scale = vec3(1, 1, 1);
	vec3 rotate_dimension;
	float angle;
	mat4 world_transform;
	bool is_Set_World_Transform = false;
	vec3 world_position;

	//Base Light Properties (All Light Types' Contained)
	base_Light Light;
	string light_type;

	//Directional Light
	vec3 direction;

	//Spot Light
	float InnerCutOffCos;
	float OuterCutOffCos;

	//Vector Place
	GLuint place_in_vector;
public:

	//Empty Initializator
	Light_Object(vector<Mesh> Meshes, string i_light_type) : Object_Model(Meshes), light_type(i_light_type) {
		if (light_type == "Point") {
			place_in_vector = Lights_Point.size();
			Lights_Point.push_back(PointLight{ vec4(world_position,1.0f), vec4(Light.color,1.0f), (vec4)Light.constant, (vec4)Light.linear, (vec4)Light.quadratic});
		}
		else if (light_type == "Directional") {
			place_in_vector = Lights_Directional.size();
			Lights_Directional.push_back(DirectionalLight{ vec4(direction, 1.0f), vec4(Light.color, 1.0f) });
		}
		else if (light_type == "Spot") {
			place_in_vector = Lights_Spot.size();
			Lights_Spot.push_back(SpotLight{ vec4(world_position, 1.0f), vec4(Light.color, 1.0f), vec4(direction, 1.0f), (vec4)InnerCutOffCos, (vec4)OuterCutOffCos, (vec4)Light.constant, (vec4)Light.linear, (vec4)Light.quadratic });
		}
	}

	void setChanges_inVector() {
		if (light_type == "Point") {
			Lights_Point[place_in_vector] = PointLight{ vec4(world_position,1.0f), vec4(Light.color,1.0f), (vec4)Light.constant, (vec4)Light.linear, (vec4)Light.quadratic};
		}
		
		else if (light_type == "Directional") {
			Lights_Directional[place_in_vector] = DirectionalLight{ vec4(direction, 1.0f), vec4(Light.color, 1.0f) };
		}
		
		else if (light_type == "Spot") {
			Lights_Spot[place_in_vector] = SpotLight{ vec4(world_position, 1.0f), vec4(Light.color, 1.0f), vec4(direction, 1.0f), (vec4)InnerCutOffCos, (vec4)OuterCutOffCos, (vec4)Light.constant, (vec4)Light.linear, (vec4)Light.quadratic };
			cout << "Direction:\n"; Read_Vec3(Lights_Spot[0].direction);
		}
	}


	// ** Most Important Note:
	//Don't forget to use this before Transform function
	//1) If you use Directional Light, set Direction as second argument
	//2) If you use Spot Light, set CutOffAngle as third argument
	//3) Function saves CutOffAngle as a COSINE
	//4) Spot Light needs Direction too, don't forget that
	void set_Light_Properties(const base_Light &properties, const vec3 &i_direction = vec3(0), const float &inner_CutOffAngle = 0, const float &outer_CutOffAngle = 0) {
		InnerCutOffCos = cos(radians(inner_CutOffAngle));
		OuterCutOffCos = cos(radians(outer_CutOffAngle));
		Light = properties;
		direction = normalize(i_direction);
		setChanges_inVector();
	}



	//If you want to scale or rotate the object, "scaled_or_Rotated" must be the scaled and/or rotated matrix
	//After that, world position must be added. Because I didn't find a way to get Light's Position data
	void Transform(const vec3 &i_scale, const vec3 &i_rotate_dimension, float i_angle, const vec3 &i_translate) {
		mat4 floor_world_transform;
		//Scale
		floor_world_transform = scale(floor_world_transform, i_scale);
		Scale = i_scale;
		//Rotate
		floor_world_transform = glm::rotate(floor_world_transform, radians(i_angle), i_rotate_dimension);
		rotate_dimension = i_rotate_dimension;
		angle = i_angle;
		//Transport
		floor_world_transform = translate(floor_world_transform, i_translate);
		world_position = i_translate;
		world_transform = floor_world_transform;
		is_Set_World_Transform = true;
		setChanges_inVector();
	}

	vec3 get_LightColor() {
		return Light.color;
	}

	vec3 get_Position() {
		return world_position;
	}

	float get_InnerCutOff() {
		return degrees(acos(InnerCutOffCos));
	}

	float get_OuterCutOff() {
		return degrees(acos(OuterCutOffCos));
	}

	//Write Light's All Properties
	//If Light has no properties specificed, it will write
	void write_Shader_Parameters() {
		cout << "Type; " << light_type << endl;
		cout << "Place in Vector; " << place_in_vector << endl;
		cout << "Position; ";
		Read_Vec3(world_position);
		cout << "Color; ";
		Read_Vec3(Light.color);
		cout << "Constant; " << Light.constant << endl;
		cout << "Linear; " << Light.linear << endl;
		cout << "Quadratic; " << Light.quadratic << endl;
		cout << "Calculate Attenuation; " << Light.calculate_Attenuation << endl;
		if (light_type == "Directional") {
			cout << "Direction; ";
			Read_Vec3(direction);
		}
		else if (light_type == "Spot") {
			cout << "Direction; ";
			Read_Vec3(direction);
			cout << "Inner CutOff Angle; " << get_InnerCutOff() << endl;
			cout << "Outer CutOffCos; " << get_OuterCutOff() << endl;
		}
	}


	bool is_Completed() {
		return is_Set_World_Transform && (this->Light.color != Sentinel_Values::normalized_Vec3);
	}

	void Draw(mat4 Proj_Matrix, mat4 View_Matrix);
};


//Render the Light's Mesh
void Light_Object::Draw(mat4 Proj_Matrix, mat4 View_Matrix) {
	if (is_Completed()) {
		for (Mesh current_Mesh : Object_Model) {
			//Sets Neccessary what is to Render
			glUseProgram(current_Mesh.Shader.ShaderProgramID);
			current_Mesh.model_world_matrix = world_transform;
			current_Mesh.set_ProjMatrix(Proj_Matrix);
			current_Mesh.set_ViewMatrix(View_Matrix);

			//Light Object Variables
			glUniform3f(current_Mesh.Shader.GetUniform("light_Color"), Light.color.x, Light.color.y, Light.color.z);

			//Actual Basic Draw Function
			current_Mesh.Draw();
		}
	}
}





/*Arguments:
1) Importer: The Mesh of Light
2) Shader: Shader of Mesh

Important Ones:
If the Arguments Below is not set true, Light won't affect any Mesh as a Light
3) Type: Point, Directional or Spot
4) Order: Which one in the list of Corresponding List Type
*/
Light_Object Import_Light(const Import &importer, ShaderProgram Shader, const string &Type) {
	vector<Mesh> Meshes;
	for (Mesh mesh : importer.allMeshes) {
		mesh.setShader(Shader);
		Meshes.push_back(mesh);
	}
	Light_Object object(Meshes, Type);
	return object;
}


//Light circles around
//Note: Circle in the Sand Parody :P
void Circle_theLight(Light_Object &light) {
	float radius = 3.0f;
	vec3 light_Position;
	light_Position.x = sin(glfwGetTime()) * radius;
	light_Position.z = cos(glfwGetTime()) * radius;
	light.Transform(vec3(1, 1, 1), vec3(1, 1, 1), 0.0f, light_Position);
}


void Attach_LightToCamera(Light_Object &light, base_Light &properties, float InnerCutOff = 0.0f, float OuterCutOff = 0.0f) {
	light.set_Light_Properties(properties, main_Camera->Front_Vector, InnerCutOff, OuterCutOff);
	light.Transform(vec3(1, 1, 1), vec3(1, 1, 1), 0.0f, main_Camera->Position);
}


//Light Uniform Buffers
GLuint UBO_Lights_Point, UBO_Lights_Directional, UBO_Lights_Spot;


/*Create Light UBOs
Binding Points are;
0) Point Light
1) Directional Light
2) Spot Light
Note: Create as 152 byte Buffers
*/
void set_LightUniform_Buffers(ShaderProgram shader) {
	unsigned int Point_Count = Lights_Point.size();
	unsigned int Directional_Count = Lights_Directional.size();
	unsigned int Spot_Count = Lights_Spot.size();
	if (Point_Count != 0) {
		//Point Lights
		unsigned int uniform_block_index = glGetUniformBlockIndex(shader.ShaderProgramID, "Lights_Point");
		glUniformBlockBinding(shader.ShaderProgramID, uniform_block_index, 1);
		glGenBuffers(1, &UBO_Lights_Point);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO_Lights_Point);
		glBufferData(GL_UNIFORM_BUFFER, (Point_Count * sizeof_PointLight), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, Point_Count * sizeof_PointLight, &Lights_Point[0]);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, UBO_Lights_Point);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	
	if (Directional_Count != 0) {
		//Directional Light
		unsigned int uniform_block_index = glGetUniformBlockIndex(shader.ShaderProgramID, "Lights_Directional");
		glUniformBlockBinding(shader.ShaderProgramID, uniform_block_index, 2);
		glGenBuffers(1, &UBO_Lights_Directional);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO_Lights_Directional);
		glBufferData(GL_UNIFORM_BUFFER, (Directional_Count * sizeof_DirectionalLight), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, Directional_Count * sizeof_DirectionalLight, &Lights_Directional[0]);
		glBindBufferBase(GL_UNIFORM_BUFFER, 2, UBO_Lights_Directional);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	
	
	if (Spot_Count != 0) {
		//Spot Light
		unsigned int uniform_block_index = glGetUniformBlockIndex(shader.ShaderProgramID, "Lights_Spot");
		glUniformBlockBinding(shader.ShaderProgramID, uniform_block_index, 3);
		glGenBuffers(1, &UBO_Lights_Spot);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO_Lights_Spot);
		glBufferData(GL_UNIFORM_BUFFER, (Spot_Count * sizeof_SpotLight), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, Spot_Count * sizeof_SpotLight, &Lights_Spot[0]);
		glBindBufferBase(GL_UNIFORM_BUFFER, 3, UBO_Lights_Spot);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	
	

}