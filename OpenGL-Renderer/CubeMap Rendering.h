#pragma once

class CubeMap_Capture {
public:
	//Right, Left, Top, Bottom, Front, Back
	CubeMap cubeMap;

	GLuint faceframebuffer_ids[6]{};
	Viewport Viewport_Info = CubeMap_Viewport;

	mat4 faceframebuffer_viewmatrixes[6];
	mat4 faceframebuffer_projmatrix;

	GLuint RendererDepthStencilBuffer;

	void Create_Depth_Stencil_RenderBuffer() {
		glGenRenderbuffers(1, &RendererDepthStencilBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, RendererDepthStencilBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, CM_Width, CM_Height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}


	CubeMap_Capture() {
		CubeMap create_CubeMap;
		cubeMap = create_CubeMap;
		glGenFramebuffers(6, faceframebuffer_ids);
		unsigned int face_int = 0;
		faceframebuffer_projmatrix = perspective(radians(CM_fov), 1.0f, 1.0f, 1000.0f);

		
		//Create the Buffers neccessary
		Create_Depth_Stencil_RenderBuffer();

		for (GLuint& face : faceframebuffer_ids) {
			//Bind the CubeMap
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.id);
			glBindFramebuffer(GL_FRAMEBUFFER, face);
			GLenum face_enum = GL_TEXTURE_CUBE_MAP_POSITIVE_X + face_int;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, face_enum, cubeMap.id, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, GL_RENDERBUFFER, RendererDepthStencilBuffer);

			//Check whether CubeMap successfully created
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
				cout << "CubeMap Buffer Creation is successful: Face " << face_int << "\n";
			}
			else if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_INVALID_ENUM){
				cout << "CubeMap Buffer Error: Not Activated: Face " << face_int << "\n";
			}
			else if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_UNSUPPORTED) {
				cout << "CubeMap Buffer Error: Unsupported Format: Face " << face_int << "\n";
			}
			else if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
				cout << "CubeMap Buffer Error: Incomplete Attachment: Face " << face_int << "\n";
			}
			else if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
				cout << "CubeMap Buffer Error: Missing Attachment: Face " << face_int << "\n";
			}
			else {
				cout << "CubeMap Buffer Error: Unknown: Face " << face_int << "\n";
			}

			face_int++;
		}


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void set_Viewport(Viewport& input) {
		Viewport_Info = input;
	}

	void Enable_CM_FrameBuffer(GLuint face) {
		CubeMap_Viewport.Activate();
		glBindFramebuffer(GL_FRAMEBUFFER, faceframebuffer_ids[face]);
		cout << "Cube Mapp needs Stencil Settings\n";
		//set_StencilTest(true, defaultStencilTest);
		
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Create_ViewMatrixes() {
		vec3 Camera_Pos = main_Camera->Position;
		//Left
		faceframebuffer_viewmatrixes[0] = lookAt(Camera_Pos, Camera_Pos + vec3(1, 0, 0), vec3(0,-1,0));
		//Right
		faceframebuffer_viewmatrixes[1] = lookAt(Camera_Pos, Camera_Pos + vec3(-1, 0, 0), vec3(0,-1,0));
		//Top
		faceframebuffer_viewmatrixes[2] = lookAt(Camera_Pos, Camera_Pos + vec3(0, 1, 0), vec3(0,0,1));
		//Bottom
		faceframebuffer_viewmatrixes[3] = lookAt(Camera_Pos, Camera_Pos + vec3(0, -1, 0), vec3(0, 0, -1));
		//Front
		faceframebuffer_viewmatrixes[4] = lookAt(Camera_Pos, Camera_Pos + vec3(0, 0, 1), vec3(0, -1, 0));
		//Back
		faceframebuffer_viewmatrixes[5] = lookAt(Camera_Pos, Camera_Pos + vec3(0, 0, -1), vec3(0, -1, 0));
	}

};

