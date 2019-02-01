#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <fstream>
#include <sstream>
using namespace std;
using namespace glm;
using namespace Assimp;

#include "Functional Header.h"
#include "Datas Header.h"
#include "Shader Class.h"
#include "Camera Class.h"
#include "Texture Class.h"
#include "FrameBuffer Class.h"
#include "Mesh Class.h"
#include "Import.h"
#include "Light Object Class.h"
#include "Mesh Object Class.h"
#include "VAO Data.h"
#include "CubeMap Rendering.h"

bool NormalMapping = false;

int main()
{
	//Create and Allocate Resources to Create Window
	create_Window();

	//Start Tests
	set_DepthTest(GL_LEQUAL, 0xFF);
	set_StencilTest(defaultStencilTest);
	set_FaceCulling(GL_CCW, GL_BACK);

	//Create Light UBOs
	//create_LightUBOs();

	//Create Shaders to Use Later
	ShaderProgram Default_Shader;
	ShaderProgram Surface_Shader_Main("Main.vert", "Main.frag");
	ShaderProgram Surface_Shader_NormalMap("Main.vert", "NormalMapping.frag");
	Surface_Shader_Main.Bind_UBOLights();
	Surface_Shader_NormalMap.Bind_UBOLights();
	ShaderProgram Post_Proccess_Default("Post_Proccess_Main.vert", "Post_Proccess_Main.frag");
	ShaderProgram Gamma_Correction("Gamma_Correction.vert", "Gamma_Correction.frag");
	ShaderProgram SkyBox_Shader("SkyBox.vert", "SkyBox.frag");
	ShaderProgram Light_1("Light.vert", "Light.frag");



	//Camera Matrix
	global_projection_matrix = perspective(radians(fov), aspect_ratio, frustum_near, frustum_far);
	Camera first_Camera(vec3(0, 0, -3));
	main_Camera = &first_Camera;


	GLuint PostProcessQuad_VAO, PostProcessQuad_VBO;
	glGenVertexArrays(1, &PostProcessQuad_VAO);
	glGenBuffers(1, &PostProcessQuad_VBO);
	glBindVertexArray(PostProcessQuad_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, PostProcessQuad_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	GLuint SkyBox_VAO, SkyBox_VBO;
	glGenVertexArrays(1, &SkyBox_VAO);
	glGenBuffers(1, &SkyBox_VBO);
	glBindVertexArray(SkyBox_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, SkyBox_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);



	//Frame and Render Buffers Creation

	//CubeMap_Capture PlayerCubeMap_Capturing;

	Texture RendererColor;
	TextureProperties MainRender_Color{ GL_TEXTURE_2D, GL_RGBA, GL_UNSIGNED_BYTE, GL_REPEAT, GL_NEAREST, GL_LINEAR, screen_Width, screen_Height };
	RendererColor.create_RenderTexture(false, 4, MainRender_Color);
	
	
	Texture RendererDepth;
	TextureProperties MainRender_Depth{ GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_FLOAT, GL_REPEAT, GL_NEAREST, GL_LINEAR, screen_Width, screen_Height };
	RendererDepth.create_RenderTexture(false, 4, MainRender_Depth);
	



	GLuint RendererObj;
	glGenFramebuffers(1, &RendererObj);
	glBindFramebuffer(GL_FRAMEBUFFER, RendererObj);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, RendererColor.texture_D, RendererColor.id, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, RendererDepth.texture_D, RendererDepth.id, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		cout << "Renderer FrameBuffer creation is successful\n\n\n\n";
	}
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
		cout << "Scene Renderer Framebuffer Error: Incomplete Attachment\n\n\n";
	}
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
		cout << "Scene Renderer Framebuffer Error: Missing Attachment\n\n\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	

	Texture AlternativeColor;
	AlternativeColor.create_RenderTexture(true, 0, MainRender_Color);

	
	Texture AlternativeDepth;
	AlternativeDepth.create_RenderTexture(true, 0, MainRender_Depth);
	
	

	GLuint AlternativeObj;
	glGenFramebuffers(1, &AlternativeObj);
	glBindFramebuffer(GL_FRAMEBUFFER, AlternativeObj);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, AlternativeColor.texture_D, AlternativeColor.id, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, AlternativeDepth.texture_D, AlternativeDepth.id, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		cout << "Alternative FrameBuffer creation is successful\n\n\n\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);



	//Texture Set
	TextureProperties RGB_texture{ GL_TEXTURE_2D, GL_RGB, GL_REPEAT, GL_NEAREST, GL_LINEAR, GL_UNSIGNED_BYTE};
	vector<Texture> ContainerTexture{ Texture("Textures/2.jpg", "texture_diffuse0"), Texture("Textures/2_Specular_Rainbow.png", "texture_specular0") };
	vector<Texture> AxeTexture{ Texture("Textures/Profile_1.jpg", "texture_diffuse0", RGB_texture) };
	vector<Texture> GrassTexture{ Texture("Textures/Grass.png", "texture_diffuse0") };
	CubeMap SkyBox_Texture(faces);

	Material_Specs Gold_Material = { vec3(0.24725f, 0.1995f, 0.0745f), vec3(0.75164f, 0.60648f, 0.22648f), vec3(0.628281f, 0.555802f, 0.366065f), float(0.4f) };
	Material_Specs Gold_Material2 = { vec3(0.2f), vec3(0.3f), vec3(0.4f), float(8) };

	cout << "Lights and Meshs are loading\n";
	unsigned int Point_Light_Number = 3;
	unsigned int Directional_Light_Number = 2;
	unsigned int Spot_Light_Number = 1;

	
	//Point Light Object
	vec3 light0_Position(0, 3, 0);
	vec3 light0_Color(0, 1, 0);
	Light_Object light_sphere0 = Import_Light(Import("C:/Users/furka/Desktop/Çalýþmalar/Modeller/Base Shapes/Cube.obj", Light_1, false), Light_1, "Point");
	base_Light properties0{ light0_Color, 1, 0.009f, 0.032f, uint(1) };
	light_sphere0.set_Light_Properties(properties0, vec3(0), 0);
	light_sphere0.Transform(vec3(1), vec3(1, 1, 1), 0.0f, light0_Position);


	//Directional Light
	vec3 Directional_Pos(0, 100, 0);
	vec3 Directional_Color(1, 0, 0);
	Light_Object Directional_Light = Import_Light(Import("C:/Users/furka/Desktop/Çalýþmalar/Modeller/Base Shapes/Cube.obj", Light_1, false), Light_1, "Directional");
	base_Light directional_properties{ Directional_Color, 1, 0.000009f, 0.0000032f, uint(1) };
	Directional_Light.set_Light_Properties(directional_properties, vec3(0, 1, 0), 0);


	//Spot Light
	vec3 Spot_Pos(0, 3, 1);
	vec3 Spot_Color(0, 0, 1);
	Light_Object Spot_Light = Import_Light(Import("C:/Users/furka/Desktop/Çalýþmalar/Modeller/Base Shapes/Cube.obj", Light_1, false), Light_1, "Spot");
	base_Light Spot_properties{ Spot_Color, 1, 0.000009f, 0.000032f, uint(1) };
	Spot_Light.set_Light_Properties(Spot_properties, vec3(0, 1, 0), 45, 50);
	light_sphere0.Transform(vec3(1), vec3(1, 1, 1), 0.0f, Spot_Pos);





	cout << "Lights are finished loading, Meshes are started\n";
	/*
	//Sponza
	mat4 Sponza_world_transform;
	Sponza_world_transform = scale(Sponza_world_transform, vec3(0.2f));
	Mesh_Object Sponza = Import_Mesh(Import("C:/Users/furka/Desktop/Çalýþmalar/Modeller/Sponza Scene/sponza.obj", Main_Shader, RGB_texture, false), Main_Shader);
	Sponza.set_World_Transform(Sponza_world_transform);
	Sponza.set_TextCoord_Sampler(1);
	Sponza.set_Material_Specs(Gold_Material2);*/
	
	
	
	//Nanosuit
	mat4 Nanosuit_world_transform;
	Nanosuit_world_transform = scale(Nanosuit_world_transform, vec3(0.5f));
	Mesh_Object Nanosuit = Import_Mesh(Import("C:/Users/furka/Desktop/Çalýþmalar/Modeller/Nanosuit/nanosuit.obj", Surface_Shader_Main, RGB_texture, false), Surface_Shader_Main);
	Nanosuit.set_World_Transform(Nanosuit_world_transform);
	Nanosuit.set_TextCoord_Sampler(1);
	Nanosuit.set_Material_Specs(Gold_Material2);
	Nanosuit.set_ProjMatrix(global_projection_matrix);
	
	
	int i = 0;
	float last = 0;
	lastframe = glfwGetTime();
	//Render Loop
	while (!glfwWindowShouldClose(window))
	{
		//Programmic Functions
		Test_FaceCulling();
		camera_Mode(main_Camera, firstMouse);
		wireframe_Transition();													//Problem: Because FPS is too high, Function determines multiple touch on one touch
		determine_RageQuit(window);
		FPS_Calculator(lastframe, true);
		set_LightUniform_Buffers(Surface_Shader_NormalMap);
		set_LightUniform_Buffers(Surface_Shader_Main);

		/*
		//Cube Map Pass
		PlayerCubeMap_Capturing.Create_ViewMatrixes();
		for (GLuint i = 0; i < 6; i++) {
			PlayerCubeMap_Capturing.Enable_CM_FrameBuffer(i);

			
			Sponza.set_ProjMatrix(mat4(mat3(PlayerCubeMap_Capturing.faceframebuffer_viewmatrixes[i])));
			Sponza.set_ViewMatrix(PlayerCubeMap_Capturing.faceframebuffer_viewmatrixes[i]);
			Sponza.Draw();
		}*/


		//Normal Scene Pass
		glViewport(0, 0, screen_Width, screen_Height);
		glBindFramebuffer(GL_FRAMEBUFFER, RendererObj);


		//Set Background
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 


		//Render SkyBox
		Stencil(Stencil_Skybox);
		glUseProgram(SkyBox_Shader.ShaderProgramID);
		glUniformMatrix4fv(SkyBox_Shader.GetUniform("view_matrix"), 1, GL_FALSE, value_ptr(mat4(mat3(main_Camera->view_matrix))));
		glUniformMatrix4fv(SkyBox_Shader.GetUniform("projection_matrix"), 1, GL_FALSE, value_ptr(global_projection_matrix));
		glBindVertexArray(SkyBox_VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBox_Texture.id);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Render MainScene Geometry Pass
		set_StencilTest(Stencil_MainPass);
		glUseProgram(Surface_Shader_Main.ShaderProgramID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBox_Texture.id);


		Nanosuit.set_Shader(Surface_Shader_NormalMap);
		Nanosuit.set_ProjMatrix(global_projection_matrix);
		Nanosuit.set_ViewMatrix(main_Camera->view_matrix);
		Nanosuit.Draw();

		//Normal Scene Rendering is Finished
		bool renderloop_wireframe = false;
		wireframe_Transition(renderloop_wireframe);

		light_sphere0.Draw(global_projection_matrix, main_Camera->view_matrix);
		Spot_Light.Draw(global_projection_matrix, main_Camera->view_matrix);

		//Multi-Sample AA Scene Texture to Normal Texture
		glBindFramebuffer(GL_READ_FRAMEBUFFER, RendererObj);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, AlternativeObj);
		glBlitFramebuffer(0, 0, screen_Width, screen_Height, 0, 0, screen_Width, screen_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBlitFramebuffer(0, 0, screen_Width, screen_Height, 0, 0, screen_Width, screen_Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		//glBlitFramebuffer(0, 0, screen_Width, screen_Height, 0, 0, screen_Width, screen_Height, GL_STENCIL_BUFFER_BIT, GL_NEAREST);		//Renderer Framebuffer hasn't any Stencil Buffer to share


		//Post Proccess Default
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(Post_Proccess_Default.ShaderProgramID);
		Post_Proccess_Default.set_Texture(0, "RenderResult", AlternativeColor.id);
		set_StencilTest(Stencil_PostProcess);
		glBindVertexArray(PostProcessQuad_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);

		/*
		//Post Process Features;
		//Gamma Correction
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		set_StencilTest(Stencil_PostProcess);
		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(PostProcess_Shader.ShaderProgramID);
		PostProcess_Shader.set_Texture(0, "RenderResult", AlternativeColor.id);
		set_StencilTest(Stencil_PostProcess);
		glBindVertexArray(PostProcessQuad_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);
		*/

		cout << "Progress: " << progress2 * 1000 << "milliseconds\n";
		glfwSwapBuffers(window);
		glfwPollEvents();
		i++;
		cout << "Loop: " << i << endl;
	}

	glfwTerminate();
	return 0;
}