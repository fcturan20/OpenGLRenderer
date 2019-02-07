#pragma once

/*
	Texture Class Implemented
	You can import Textures when you initialize a Texture object with an Path
	Or, you can use add an Name to only use in an Specific Shader
	But, if you want to set all Wrapping, Filtering operations by yourself, you can add them in initialization

*/

struct TextureProperties {
	GLint texture_D = GL_TEXTURE_2D, Texture_Format = GL_RGBA, ValueType = GL_UNSIGNED_BYTE;
	GLint Wrapping = GL_REPEAT, Mipmap_Filtering = GL_NEAREST, Upscale_Filtering = GL_LINEAR;
	int width = 1, height = 1;

	void Log() {
		cout << "Width: " << width << endl;
		cout << "Height: " << height << endl;
		if (texture_D == GL_TEXTURE_2D) {
			cout << "Non-Multisampled 2D Texture\n";
		}
		else if (texture_D == GL_TEXTURE_2D_MULTISAMPLE) {
			cout << "Multisampled 2D Texture\n";
		}
		if (Texture_Format == GL_RGBA) {
			cout << "RGBA colored\n";
		}
		else if (Texture_Format == GL_DEPTH_COMPONENT) {
			cout << "Depth Texture\n";
		}
		cout << "Info ended\n\n";
	}
};

class Texture {
public:
	GLuint  MSAASamples = 0;
	GLint texture_D = GL_TEXTURE_2D, Texture_Format = GL_RGBA, ValueType = GL_UNSIGNED_BYTE;
	GLint Wrapping = GL_REPEAT, Mipmap_Filtering = GL_NEAREST, Upscale_Filtering = GL_LINEAR;
	int width, height, channels;
	const char* path;
	unsigned int id;
	string string_name = "texture_diffuse0";

	void set_TextureProperties(TextureProperties properties) {
		texture_D = properties.texture_D;
		Texture_Format = properties.Texture_Format;
		ValueType = properties.ValueType;
		Wrapping = properties.Wrapping;
		Mipmap_Filtering = properties.Mipmap_Filtering;
		Upscale_Filtering = properties.Upscale_Filtering;
	}

	void set_TextureResolution(TextureProperties properties) {
		width = properties.width;
		height = properties.height;
	}

	void Set_Texture() {
		const char *i_name = string_name.c_str();
		
		glBindTexture(texture_D, id);
		// Set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(texture_D, GL_TEXTURE_WRAP_S, Wrapping);
		glTexParameteri(texture_D, GL_TEXTURE_WRAP_T, Wrapping);
		glTexParameteri(texture_D, GL_TEXTURE_MIN_FILTER, Mipmap_Filtering);
		glTexParameteri(texture_D, GL_TEXTURE_MAG_FILTER, Upscale_Filtering);
		// Load and Generate the Texture
		unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
		if (data)
		{
			if (channels == 3) {
				Texture_Format = GL_RGB;
			}
			else if (channels == 4) {
				Texture_Format = GL_RGBA;
			}
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D(texture_D, 0, Texture_Format, width, height, 0, Texture_Format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(texture_D);
		}
		else
		{
			std::cout << "Failed to load texture: " << path << std::endl;
		}
		stbi_image_free(data);
		glBindTexture(texture_D, 0);
	}


	Texture(const char* path) : path(path) {
		glGenTextures(1, &id);
		Set_Texture();
	}

	Texture(const char *path, string i_name) : path(path), string_name(i_name) {
		glGenTextures(1, &id);
		Set_Texture();
	}

	Texture(const char *path, string i_name, int texture_dimension, int i_Wrapping, int i_Mipmap_Filtering, int i_Upscale_Filtering, int i_Color_Format) :
		path(path), string_name(i_name), texture_D(texture_dimension), Wrapping(i_Wrapping), Mipmap_Filtering(i_Mipmap_Filtering), Upscale_Filtering(i_Upscale_Filtering), Texture_Format(i_Color_Format) {
		glGenTextures(1, &id); 
		Set_Texture();
	}

	Texture(const char *path, string i_name, const TextureProperties &properties) : path(path), string_name(i_name) {
		glGenTextures(1, &id);
		set_TextureProperties(properties);
		Set_Texture();
	}


	//Create Render Target or Not Specified Texture
	Texture() {
		glGenTextures(1, &id);
	}

	/*Notes:
	1) If MSAA won't be used, MSAASamples has to be set 0
	2) Texture Resolution is defined in TextureProperties
	*/
	Texture* create_RenderTexture(bool create_MipMap, GLuint i_MSAASamples, TextureProperties properties) {
		set_TextureProperties(properties);
		set_TextureResolution(properties);
		MSAASamples = i_MSAASamples;

		if (MSAASamples) {
			texture_D = GL_TEXTURE_2D_MULTISAMPLE;
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);

			// Set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, Wrapping);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, Wrapping);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAASamples, Texture_Format, width, height, GL_TRUE);

			
			//Create MipMap
			if (create_MipMap) {
				glGenerateMipmap(GL_TEXTURE_2D_MULTISAMPLE);
				glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, Mipmap_Filtering);
				glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, Upscale_Filtering);
			}
			

			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		}
		else {
			glGenTextures(1, &id);
			glBindTexture(texture_D, id);

			// Set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(texture_D, GL_TEXTURE_WRAP_S, Wrapping);
			glTexParameteri(texture_D, GL_TEXTURE_WRAP_T, Wrapping);
			glTexImage2D(texture_D, 0, Texture_Format, width, height, 0, Texture_Format, ValueType, 0);

			//Create MipMap
			if (create_MipMap) {
				glGenerateMipmap(texture_D);
				glTexParameteri(texture_D, GL_TEXTURE_MIN_FILTER, Mipmap_Filtering);
				glTexParameteri(texture_D, GL_TEXTURE_MAG_FILTER, Upscale_Filtering);
			}

			glBindTexture(texture_D, 0);
		}
		return this;
	}


	void setMipMapManually(GLint Mipmap_Level, const char* mipmap_texture) {
		glBindTexture(texture_D, id);
		glTexImage2D(texture_D, Mipmap_Level, Texture_Format, width, height, 0, Texture_Format, GL_UNSIGNED_BYTE, mipmap_texture);
	}

	void setWrapping(const GLint &setWrapping) {
		Wrapping = setWrapping;
		Set_Texture();
	}

	void setFiltering(const GLint &Downscale, const GLint &Upscale) {
		Mipmap_Filtering = Downscale;
		Upscale_Filtering = Upscale;
		Set_Texture();
	}
};


class Framebuffer;

//SPP means Sample Per Pixel (MSAA)
//Create Mipmaps
class RenderTarget {
protected:
	void set_Resolution(int width, int height);

public:
	GLuint id;
	TextureProperties Properties;
	bool create_MipMaps = false;
	GLint SPP;


	inline RenderTarget() {  Generate_ID();  }
	RenderTarget(TextureProperties in, GLint MSAA_SPP, bool should_MipMaps = false) : Properties(in), SPP(MSAA_SPP), create_MipMaps(should_MipMaps){
		Generate_ID(); set_MSAA(SPP);  //-> "Set_MSAA()" generated the Texture
	}
	inline void Log() { Properties.Log(); }
	void set_MSAA(GLint spp);
	friend class Framebuffer;

	//Side Functions
private:
	inline void Generate_ID() { glGenTextures(1, &id); }
	void Generate_RenderTarget();


};

void RenderTarget::Generate_RenderTarget() {
	glBindTexture(Properties.texture_D, id);

	// Set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(Properties.texture_D, GL_TEXTURE_WRAP_S, Properties.Wrapping);
	glTexParameteri(Properties.texture_D, GL_TEXTURE_WRAP_T, Properties.Wrapping);
	
	/*MSAA*/		if (SPP > 0) {  glTexImage2DMultisample(Properties.texture_D, SPP, Properties.Texture_Format, Properties.width, Properties.height, GL_TRUE);  }
	/*Not MSAA*/	else {  glTexImage2D(Properties.texture_D, 0, Properties.Texture_Format, Properties.width, Properties.height, 0, Properties.Texture_Format, Properties.ValueType, 0);  }


	//Create MipMap
	if (create_MipMaps) {
		glGenerateMipmap(Properties.texture_D);
		glTexParameteri(Properties.texture_D, GL_TEXTURE_MIN_FILTER, Properties.Mipmap_Filtering);
		glTexParameteri(Properties.texture_D, GL_TEXTURE_MAG_FILTER, Properties.Upscale_Filtering);
	}


	glBindTexture(Properties.texture_D, 0);
}


//Set MSAA and Generates a texture according to Properties
void RenderTarget::set_MSAA(GLint MSAA_SPP) {
	SPP = MSAA_SPP;
	if (SPP > 0) { Properties.texture_D = GL_TEXTURE_2D_MULTISAMPLE; }
	Generate_RenderTarget();
}

void RenderTarget::set_Resolution(int width, int height) {
	Properties.width = width;
	Properties.height = height;
	Generate_RenderTarget();
}



class CubeMap {
public:
	int width, height, channels;
	vector<const char*> paths;
	unsigned int id;
	string string_name = "texture_diffuse0";
	const char *name = string_name.c_str();

	void Set_Texture() {
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
		uint i = 0;
		for (const char* path : paths) {
			// Load the Textures per CubeMap Face
			unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
			if (data)
			{
				stbi_set_flip_vertically_on_load(false);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else
			{
				std::cout << "Failed to load texture: " << path << std::endl;
			}
			stbi_image_free(data);
			i++;
		}

		// Set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}


	CubeMap(vector<const char*> paths) : paths(paths) {
		glGenTextures(1, &id);
		Set_Texture();
	}

	CubeMap() {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
		for (GLuint i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, CM_Width, CM_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		}

		// Set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void Bind_theCubeMap() {
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	}
};
