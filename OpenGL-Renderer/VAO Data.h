#pragma once
#include <iostream>

float quadVertices[] = {  
// positions   // texCoords
-1.0f, 1.0f, 0.0f, 1.0f,
-1.0f, -1.0f, 0.0f, 0.0f,
1.0f, -1.0f, 1.0f, 0.0f,

-1.0f, 1.0f, 0.0f, 1.0f,
1.0f, -1.0f, 1.0f, 0.0f,
1.0f, 1.0f, 1.0f, 1.0f
};


vector<const char*> faces
{
	"Textures/CubeMap/right.jpg",
	"Textures/CubeMap/left.jpg",
	"Textures/CubeMap/top.jpg",
	"Textures/CubeMap/bottom.jpg",
	"Textures/CubeMap/front.jpg",
	"Textures/CubeMap/back.jpg"
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

vector <vec3> vertices_cube {
	//Positions
	vec3(-0.5f, -0.5f, -0.5f),				
	vec3(-0.5f, 0.5f, -0.5f),					
	vec3(0.5f, 0.5f, -0.5f),					
	vec3(0.5f, -0.5f, -0.5f),					
	vec3(-0.5f, -0.5f, 0.5f),					
	vec3(-0.5f, 0.5f, 0.5f),					
	vec3(0.5f, 0.5f, 0.5f),					
	vec3(0.5f, -0.5f, 0.5f)			
};

vector <vec3> colors_cube{
	vec3(0,0,1),
	vec3(1,0,0),
	vec3(0,1,0),
	vec3(1,1,1),
	vec3(1,0,1),
	vec3(1,1,0),
	vec3(0,1,1),
	vec3(0,1,0)
};


vector <unsigned int> indices_cube {
	0,1,2,		//Front Face
	0,2,3,
	0,4,5,		//Left Face
	0,1,5,
	3,2,6,		//Right Face
	3,6,7,
	0,3,7,		//Down Face
	0,4,7,
	1,2,6,		//Up Face
	1,5,6,
	4,5,6,		//Back Face
	4,6,7
};

vector <vec2> textCoord_Cube{
	vec2(0.0f,0.0f),
	vec2(0.0f,1.0f),
	vec2(1.0f,1.0f),
	vec2(1.0f,0.0f),
	vec2(2.0f,0.0f),
	vec2(0.0f,2.0f),
	vec2(2.0f,2.0f),
	vec2(2.0f,0.0f)
};

//Light Object Vertex Data
const float lightCube_vertices1[]{
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, 0.5f
};

const unsigned int lightCube_indices1[]{
	0,1,2,		//Front Face
	0,2,3,
	0,4,5,		//Left Face
	0,1,5,
	3,2,6,		//Right Face
	3,6,7,
	0,3,7,		//Down Face
	0,4,7,
	1,2,6,		//Up Face
	1,5,6,
	4,5,6,		//Back Face
	4,6,7
};


/*	//Cube Class to Test Things, but Texture Coords are issue
class Cube_Class {
private:
	vec3 scale = vec3(1, 1, 1);
	float rotationAngle = 0.0f;
	vec3 rotation_Direction = vec3(0, 0, 0);
	vec3 position = vec3(0, 0, 0);
	ShaderProgram shader = ShaderProgram("Main.vert", "Main.frag");
public:

	Cube_Class(vec3 i_scale, float i_rotationAngle, vec3 i_rotation_Direction, vec3 i_position)
		: scale(i_scale), rotationAngle(i_rotationAngle), rotation_Direction(i_rotation_Direction), position(i_position) {

		mat4 world_transform;
		world_transform = glm::scale(world_transform, scale);
		world_transform = glm::rotate(world_transform, radians(rotationAngle), rotation_Direction);
		world_transform = translate(world_transform, position);

		vector<Texture> LarasAxe{ Texture("Textures/Profile_1.jpg", "texture_diffuse0") };
		Mesh Cube(cube_Vertex_Data(), indices_cube, LarasAxe, shader);
		Cube.model_world_matrix = world_transform;
		Cube.Draw();
	}

	//Set Cube's Basic Vertices
	vector<Vertex> cube_Vertex_Data() {
		vector <Vertex> cube_Vertex;

		for (int i = 0; i < 8; i++) {
			Vertex current_Vertex;
			current_Vertex.Position = vertices_cube[i];
			current_Vertex.TextCoord = textCoord_Cube[i];
			current_Vertex.Normal = vec3(1, 0, 0);
			cube_Vertex.push_back(current_Vertex);
		}

		return cube_Vertex;
	}

	void Draw(Mesh mesh) {

		mesh.Draw();
	}
};*/