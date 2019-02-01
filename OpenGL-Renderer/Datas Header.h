#pragma once
#include <iostream>

//Vertex Datas
vector<float> vertices1{
	//Positions			//Colors	//TextCoord
	-0.5f, 0.0f, 0,		1,0,0,		0.0f,0.0f,
	0.0f , 0.5f, 0,		0,1,0,		2.0f,2.0f,
	0.5f, 0.0f, 0,		0,0,1,		4.0f,0.0f

};
const float vertices2[]{
	//Positions					//Colors	//TextCoord
	-0.5f, -0.5f, -0.5f,		0,0,1,		0.0f,0.0f,
	-0.5f, 0.5f, -0.5f,			1,0,0,		0.0f,1.0f,
	0.5f, 0.5f, -0.5f,			0,1,0,		1.0f,1.0f,
	0.5f, -0.5f, -0.5f,			1,1,1,		1.0f,0.0f,
	-0.5f, -0.5f, 0.5f,			0,0,1,		2.0f,0.0f,
	-0.5f, 0.5f, 0.5f,			1,0,0,		0.0f,2.0f,
	0.5f, 0.5f, 0.5f,			0,1,0,		2.0f,2.0f,
	0.5f, -0.5f, 0.5f,			1,1,1,		2.0f,0.0f
};

//Indices Datas
vector<float> indices1{
	0,1,2
};
const unsigned int indices2[]{
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

//Texture Coordinates
vector<float> textcoord1{
	0,0,
	0.5f, 0.5f,
	1,0
};
float textcoord2[]{
	0,0,
	0,1,
	1,1,
	1,0
};


glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};