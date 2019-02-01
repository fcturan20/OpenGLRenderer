#pragma once
/*

//Defaults;
//Stencil buffer doesn't support Render to Texture
//Color and Depth Buffers doesn't Render to Texture
//We use RenderBuffer as default
//Viewport Settings are default (At Functional Header)
//MSAA is off as default (You can enable it)
class Framebuffer {
private:
	GLuint FrameBuffer_ID, Color_ID, Depth_ID;
	Viewport Viewport_Info;
	Stencil StencilSettings;

	//Render Textures if you want
	Texture ColorTexture, DepthTexture;
	TextureProperties ColorTextProperties, DepthTextProperties;
	bool create_MipMap = false;

	GLuint MSAA = 0;
	bool Color_RenderToTexture, Depth_RenderToTexture;
	bool is_Stencil = false, is_PostProcess = false;
public:
	Framebuffer() {
		Create_Buffer();
	}
	Framebuffer(Viewport Viewport = Current_Viewport, bool i_Color_RenderToTexture = false, bool i_Depth_RenderToTexture = false, bool i_Stencil = false, bool i_PostProcess = false) : 
		Viewport_Info(Viewport), Color_RenderToTexture(i_Color_RenderToTexture), Depth_RenderToTexture(i_Depth_RenderToTexture) ,is_Stencil(i_Stencil), is_PostProcess(i_PostProcess)
	{
		
	}

	//Create a Buffer ID
	void Create_Buffer() {
		glGenFramebuffers(1, &FrameBuffer_ID);
	}

	//Create and Activate RenderToTexture(s)
	//This buffers will be renderer to textures
	void Create_RenderToTexture_s(bool Color, bool Depth, TextureProperties ColorProperties, TextureProperties DepthProperties) {
		ColorTextProperties = ColorProperties;
		DepthTextProperties = DepthProperties;
		if (Color) {
			ColorTexture.create_RenderTexture(create_MipMap, MSAA, ColorProperties);
		}
	}

	//Enable the Framebuffer to use
	//If you clear, all buffers inside will be cleared to value of 1
	void Activate(bool should_Clear = false) {
		//Viewport Settings
		if (!(Current_Viewport.at_X == Viewport_Info.at_X && Current_Viewport.at_Y == Viewport_Info.at_Y && Current_Viewport.Width == Viewport_Info.Width && Current_Viewport.Height == Viewport_Info.Height)) {
			cout << "Viewport is set\n";
			glViewport(Viewport_Info.at_X, Viewport_Info.at_Y, Viewport_Info.Width, Viewport_Info.Height);
		}
		else {
			cout << "Viewport is already set\n";
		}

		//Bind Framebuffer and Stencil Settings if Necessary
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer_ID);
		if (is_Stencil) {
			set_StencilTest(StencilSettings);
		}
		if (should_Clear) {
			glClearColor(1, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}
	}

	//Clear buffers you want
	void ClearBuffers(bool Color = true, bool Depth = true, bool Stencil = true) {
		glClearColor(1, 1, 1, 1);
		if (Color) {
			glClear(GL_COLOR_BUFFER_BIT);
		}
		if (Depth) {
			glClear(GL_DEPTH_BUFFER_BIT);
		}
		if (Stencil) {
			glClear(GL_STENCIL_BUFFER_BIT);
		}
	}

	//Copy Settings from Right One to Left One
	//Doesn't send Buffers, if you want to send;
	//Call Receive Buffers and Add Wanted Buffers as Argument
	Framebuffer& operator=(Framebuffer &copy_To) {
		copy_To.MSAA = MSAA;
		copy_To.Viewport_Info = Viewport_Info;
		copy_To.StencilSettings = StencilSettings;
		copy_To.is_Stencil = is_Stencil;
		copy_To.is_PostProcess = is_PostProcess;
	}

	void Receive_Buffers(GLuint BufferID_copy_From) {
		//Blit Framebuffer
	}


};

*/