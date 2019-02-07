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
	Viewport Viewport_Info = Current_Viewport;
	Stencil StencilSettings = currentStencilTest;

	//Render Textures if you want
	RenderTarget *ColorTexture, *DepthTexture;
	TextureProperties ColorTextProperties, DepthTextProperties;
	int width = screen_Width, height = screen_Height;
	bool create_MipMap = false;

	GLuint MSAA = 0;
	bool Color_RenderToTexture = false, Depth_RenderToTexture = false;
	bool is_Stencil = false, is_PostProcess = false;
public:
	GLuint FrameBuffer_ID, Color_ID, Depth_ID;
	Framebuffer() {
		Create_Buffer();
	}
	/*
	Framebuffer(Viewport Viewport = Current_Viewport, bool i_Color_RenderToTexture = false, bool i_Depth_RenderToTexture = false, bool i_Stencil = false, bool i_PostProcess = false) : 
		Viewport_Info(Viewport), Color_RenderToTexture(i_Color_RenderToTexture), Depth_RenderToTexture(i_Depth_RenderToTexture) ,is_Stencil(i_Stencil), is_PostProcess(i_PostProcess)
	{
		
	}
	
	//Bind input texture to Framebuffer
	void set_RenderTexture(RenderTarget* RT) {
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer_ID);
		if (RT->Properties.Texture_Format == GL_RGBA) {
			cout << "Color Render Texture is set\n";
			ColorTextProperties = TextureProperties{ RT->Properties.texture_D, RT->Properties.Texture_Format, RT->Properties.ValueType, RT->Properties.Wrapping, RT->Properties.Mipmap_Filtering, RT->Properties.Upscale_Filtering, RT->Properties.width, RT->Properties.height };
			ColorTexture = RT;
			Color_ID = RT->id;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ColorTexture->Properties.texture_D, ColorTexture->id, 0);
			Color_RenderToTexture = true;
		}
		else if (RT->Properties.Texture_Format == GL_DEPTH_COMPONENT) {
			cout << "Depth Render Texture is set\n";
			DepthTextProperties = TextureProperties{ RT->Properties.texture_D, RT->Properties.Texture_Format, RT->Properties.ValueType, RT->Properties.Wrapping, RT->Properties.Mipmap_Filtering, RT->Properties.Upscale_Filtering, RT->Properties.width, RT->Properties.height };
			DepthTexture = RT;
			Depth_ID = RT->id;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, DepthTexture->Properties.texture_D, DepthTexture->id, 0);
			Depth_RenderToTexture = true;
		}
	}

	void check_Errors() {
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
			cout << "FrameBuffer creation is successful\n\n\n\n";
		}
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
			cout << "Framebuffer Error: Incomplete Attachment\n\n\n";
		}
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
			cout << "Framebuffer Error: Missing Attachment\n\n\n";
		}
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
			Texture* x = new Texture(ColorProperties, (GLint)4, false);
			ColorTexture = x;
		}
		if (Depth) {
			RenderTarget* x = new RenderTarget(DepthProperties, (GLint)4, false);
			DepthTexture = x;
		}
	}
	

	void set_Viewport(Viewport& input) {
		Viewport_Info = input;
	}

	//Enable the Framebuffer to use
	//If you clear, all buffers inside will be cleared to value of 1
	void Activate(bool should_Clear = false) {
		//Viewport Settings
		//This very long "if" checks if active Viewport Settings is same with class' Viewport Settings

		//Bind Framebuffer and Stencil Settings if Necessary
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer_ID);
		/*
		if (is_Stencil) {
			StencilSettings.Activate();
		}
		if (should_Clear) {
			glClearColor(1, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}
	}

	//Clear buffers you want
	void ClearBuffers(bool Color = true, bool Depth = true, bool Stencil = true) {
		glBindBuffer(GL_FRAMEBUFFER, FrameBuffer_ID);
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

	//Copy Settings
	Framebuffer& operator=(Framebuffer &copy_From) {
		MSAA = copy_From.MSAA;
		Viewport_Info = copy_From.Viewport_Info;
		StencilSettings = copy_From.StencilSettings;
		is_Stencil = copy_From.is_Stencil;
		is_PostProcess = copy_From.is_PostProcess;
		Receive_Buffers(copy_From);
		return *this;
	}


	//Blit Framebuffer
	void Receive_Buffers(Framebuffer& copy_From) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, copy_From.FrameBuffer_ID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FrameBuffer_ID);


		if (Color_RenderToTexture && copy_From.Color_RenderToTexture) {
			cout << "Color Textures are Blitting\n";
			glBlitFramebuffer(0, 0, 1920, 1080,
				0, 0, 1920, 1080, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		}


		if (Depth_RenderToTexture && copy_From.Depth_RenderToTexture) {
			cout << "Depth Textures are Blitting\n";
			#define from_Viewport copy_From.Viewport_Info
			#define to_Viewport Viewport_Info
			glBlitFramebuffer(0, 0, 1920, 1080,
				0, 0, 1920, 1080, GL_DEPTH_ATTACHMENT, GL_NEAREST);
			#define from_Viewport
			#define to_Viewport
		}
		
	}


};
*/


class Framebuffer {
	GLuint MSAA_SPP;
	Viewport FB_viewport;
public:
	GLuint id;
	RenderTarget *FB_Color, *FB_Depth;

	Framebuffer() {
		Generate_ID();
	}

	Framebuffer(RenderTarget* Color, RenderTarget* Depth, Viewport viewport) : FB_viewport(viewport) {
		if (Color->SPP == Depth->SPP) {
			FB_Color = Color;
			FB_Depth = Depth;
		}
		else {
			cout << "RenderTarget have different MSAA settings!\nDeleting Framebuffer\n";
		}
		
		if ((Color == nullptr) || (Depth == nullptr)) {
			cout << "Null RenderTarget passes so deleting Framebuffer\n";
			delete this;
		}

		if ((Color->Properties.width != Depth->Properties.width) || (Color->Properties.height != Depth->Properties.height)) {
			cout << "RenderTargets have different Resolutions!\nDeleting Framebuffer\n";
		}

		int cx = viewport.Height;
		int cy = viewport.Width;
		if ((cx != Color->Properties.height) || (cx != Depth->Properties.height) || (cy != Color->Properties.width) || (cy != Depth->Properties.width)) {
			cout << "One of RenderTargets have different Resolution from Viewport\nDeleting Framebuffer\n";
		}


		Generate_ID();
		set_ColorRT();
		set_DepthRT();
		check_Errors();
	}

	void Activate() {
		//Check whether Resolutions are same or not
		if ((FB_Color->Properties.width != FB_Depth->Properties.width) || (FB_Color->Properties.height != FB_Depth->Properties.height)) {
			cout << "RenderTargets have different Resolutions!\nDeleting Framebuffer\n";
			delete this;
			return;
		}
		//Check whether MSAA settings are same or not
		else if(FB_Color->SPP != FB_Depth->SPP){
			cout << "RenderTargets have different MSAA settings!\nDeleting Framebuffer\n";
			delete this;
			return;
		}
		//Activate Framebuffer
		else {
			//First, Viewport is activated
			FB_viewport.Activate();
			glBindFramebuffer(GL_FRAMEBUFFER, id);
			cout << "Framebuffer Viewport is activated\n";
		}
	}
	void Create_Buffers() {

	}
	//If you want to apply Resolution changes in your
	void update_Viewport(Viewport in) {
		FB_Color->set_Resolution(in.Width, in.Height);
		FB_Depth->set_Resolution(in.Width, in.Height);

		FB_viewport = in;
	}
	//Copy Settings
	Framebuffer& operator=(Framebuffer &copy_From) {
		MSAA_SPP = copy_From.MSAA_SPP;
		FB_viewport = copy_From.FB_viewport;
		Receive_Buffers(copy_From);
		return *this;
	}


	//Blit Framebuffer
	void Receive_Buffers(Framebuffer& copy_From) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, copy_From.id);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);

		cout << "Color Textures are Blitting\n";
		glBlitFramebuffer(copy_From.FB_viewport.at_X, copy_From.FB_viewport.at_Y, copy_From.FB_viewport.Width, copy_From.FB_viewport.Height,
			FB_viewport.at_X, FB_viewport.at_Y, FB_viewport.Width, FB_viewport.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		cout << "Depth Textures are Blitting\n";
		glBlitFramebuffer(copy_From.FB_viewport.at_X, copy_From.FB_viewport.at_Y, copy_From.FB_viewport.Width, copy_From.FB_viewport.Height,
			FB_viewport.at_X, FB_viewport.at_Y, FB_viewport.Width, FB_viewport.Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	}

	void check_Errors() {
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
			cout << "FrameBuffer creation is successful\n\n\n\n";
		}
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
			cout << "Framebuffer Error: Incomplete Attachment\n\n\n";
		}
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
			cout << "Framebuffer Error: Missing Attachment\n\n\n";
		}
	}
private:
	void Generate_ID() {
		glGenFramebuffers(1, &id);
	}
	
	
	void set_ColorRT() {
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, FB_Color->Properties.texture_D, FB_Color->id, 0);
	}
	void set_DepthRT() {
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, FB_Depth->Properties.texture_D, FB_Depth->id, 0);
	}
};