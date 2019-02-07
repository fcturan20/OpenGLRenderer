#pragma once
#include <iostream>

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