#pragma once

//Global Variables
int screen_Width(1920), screen_Height(1080);
float mixture;
float mouse_last_x(screen_Width / 2), mouse_last_y(screen_Height / 2), mouse_yaw(0.0f), mouse_pitch(0.0f);
float fov(45), aspect_ratio((float)screen_Width / (float)screen_Height);
bool firstMouse = true;
mat4 global_projection_matrix;
GLFWwindow* window;
float lastframe;
bool wireframe = false, temporary_wireframe = false;
float frustum_near = 0.1f, frustum_far = 10000.0f;
float Delta_Time = 1;
float progress = 0;

//Global CubeMap Variables 
int CM_Width = int(240), CM_Height = int(240);
float CM_fov(90), CM_aspect_ratio((float)CM_Width / (float)CM_Height);

//Global Sentinel Values (Namespace is to find Easily)
namespace Sentinel_Values {
	const vec3 normalized_Vec3(-2, -2, -2);
	const vec4 normalized_Vec4(-2);
	const float attenuation_constant(-1);
	const float attenuation_linear(-1);
	const float attenuation_quadratic(-1);
	const float cos_Inner_CutOff(-1);
	const float cos_Outer_CutOff(-1);
}



struct Viewport {
	GLint at_X, at_Y;
	int Width, Height;

	void Log() {
		cout << "At_X: " << at_X << endl;
		cout << "At_Y: " << at_Y << endl;
		cout << "Width: " << Width << endl;
		cout << "Height: " << Height << endl;
	}

	bool operator==(Viewport& equal) {
		//They are not same!
		if (!(equal.at_X == at_X && equal.at_Y == at_Y && equal.Width == Width && equal.Height == Height)) {
			return false;
		}
		else {
			return true;
		}
	}

	bool operator!=(Viewport& equal) {
		return (!(equal == *this));
	}


	Viewport& operator=(Viewport &copy_From) {
		at_X = copy_From.at_X;
		at_Y = copy_From.at_Y;
		Width = copy_From.Width;
		Height = copy_From.Height;
		return *this;
	}

	void Activate();
};


Viewport get_WindowViewport() {
	GLint Viewport_Infos[4];
	glGetIntegerv(GL_VIEWPORT, Viewport_Infos);
	return Viewport { Viewport_Infos[0], Viewport_Infos[1], Viewport_Infos[2], Viewport_Infos[3] };
}

Viewport Main_Viewport{
	0,0,1920,1080
};

Viewport Current_Viewport(Main_Viewport);
Viewport CubeMap_Viewport{0, 0, CM_Width, CM_Height};

void Viewport::Activate() {
	cout << "Activated Viewport:\n";
	cout << "At_X: " << at_X << endl;
	cout << "At_Y: " << at_Y << endl;
	cout << "Width: " << Width << endl;
	cout << "Height: " << Height << endl;
	glViewport(at_X, at_Y, Width, Height);
	
}


//Set Window Size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	cout << "Width: " << width << endl;
	cout << "Height: " << height << endl;
	glViewport(0, 0, width, height);
}


void FPS_Calculator(float &Last_Frame, const bool &show_FPS) {
	//FPS Calculation
	float get_Time = (float)glfwGetTime();
	float Delta_Time = get_Time - Last_Frame;
	float FPS = 1.0f / Delta_Time;
	Last_Frame = get_Time;
	if (show_FPS) {
		cout << "Current Frame: " << Delta_Time * 1000 << " millisecond\n";
		cout << FPS << " FPS\n" << endl;
	}
}

//Main Calculator
void FPS_Calculator(const bool &show_FPS) {
	//FPS Calculation
	float get_Time = (float)glfwGetTime();
	float delta_Time = get_Time - lastframe;
	Delta_Time = delta_Time;
	float FPS = 1.0f / delta_Time;
	lastframe = get_Time;
	if (show_FPS) {
		cout << "Current Frame: " << delta_Time * 1000 << " millisecond\n";
		cout << FPS << " FPS\n" << endl;
	}
}



//If the user wants Rage Quit
void determine_RageQuit(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}



//Mouse Callback Functions
//Set Mouse Controllers Active
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

}

void mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		mouse_last_x = xpos;
		mouse_last_y = ypos;
	}
	float xoffset = xpos - mouse_last_x;
	float yoffset = mouse_last_y - ypos;
	mouse_last_x = xpos;
	mouse_last_y = ypos;

	float sensitivity(0.05f);
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	mouse_yaw += xoffset;
	mouse_pitch += yoffset;

	if (mouse_pitch > 89.0f) {
		mouse_pitch = 89.0f;
	}
	else if (mouse_pitch < -89.0f) {
		mouse_pitch = -89.0f;
	}
}


void create_Window() {
	//GLFW'yi baþlatma kodlarý
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(screen_Width, screen_Height, "Learn OpenGL", NULL, NULL);


	//Check and Report if GLFW fails
	if (window == NULL) {
		cout << "We failed to create the window because of GLFW" << endl;
		glfwTerminate();
	}

	//Set the settings of Window
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);	//V-Sync Enabled
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_position_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//Check and Report if GLAD fails
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "We failed to create the window because of GLAD" << endl;
	}
	glViewport(0, 0, 1920, 1080);

	cout << "Create window finished!\n";
	//Functional Variables
	lastframe = 0.0f;
}


void set_DepthTest(GLint Comparison_Way = GL_LESS, GLint Write_toBuffer = GL_TRUE) {
	//Enable the Depth Test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(Comparison_Way);
	glDepthMask(Write_toBuffer);
}



struct Stencil {
public:
	int Comparison_Way = GL_ALWAYS, Write_toBuffer = 0xFF, Stencil_Value = 1, Buffer_Value = 0xFF, 	//Argument List
		When_StencilFails = GL_KEEP, When_DepthFails = GL_KEEP, When_AllPasses = GL_KEEP,
		Depth_Test = true, Depth_Comparison = GL_LEQUAL, Write_toDepthBuffer = GL_TRUE;

	Stencil(int i_Comparison_Way = GL_ALWAYS, int i_Write_toBuffer = 0xFF, int i_Stencil_Value = 1, int i_Buffer_Value = 0xFF, int i_When_StencilFails = GL_KEEP,
		int i_When_DepthFails = GL_KEEP, int i_When_AllPasses = GL_REPLACE, int i_Depth_Test = true, int i_Depth_Comparison = GL_LEQUAL, int i_Write_toDepthBuffer = GL_TRUE) :
		Comparison_Way(i_Comparison_Way), Write_toBuffer(i_Write_toBuffer), Stencil_Value(i_Stencil_Value), Buffer_Value(i_Buffer_Value), When_StencilFails(i_When_StencilFails),
		When_DepthFails(i_When_DepthFails), When_AllPasses(i_When_AllPasses), Depth_Test(i_Depth_Test), Depth_Comparison(i_Depth_Comparison), Write_toDepthBuffer(i_Write_toDepthBuffer)
	{
		//All variables are set allready, no code here
	}

	bool operator==(const Stencil& compare)
	{
		if (Comparison_Way == compare.Comparison_Way && Write_toBuffer == compare.Write_toBuffer && Stencil_Value == compare.Stencil_Value && Buffer_Value == compare.Buffer_Value && 	//Argument List
			When_StencilFails == compare.When_StencilFails && When_DepthFails == compare.When_DepthFails && When_AllPasses == compare.When_AllPasses &&
			Depth_Test == compare.Depth_Test && Depth_Comparison == compare.Depth_Comparison && Write_toDepthBuffer == compare.Write_toDepthBuffer)
		{
			//cout << "Stencil Settings are same\n";
			return true;
		}
		else {
			//cout << "Stencil Settings are same\n";
			return false;
		}
	}

	Stencil& operator=(Stencil& copy_From) {
		Comparison_Way = copy_From.Comparison_Way; Write_toBuffer = copy_From.Write_toBuffer; Stencil_Value = copy_From.Stencil_Value; Buffer_Value == copy_From.Buffer_Value;
		When_StencilFails = copy_From.When_StencilFails; When_DepthFails = copy_From.When_DepthFails; When_AllPasses = copy_From.When_AllPasses;
		Depth_Test = copy_From.Depth_Test; Depth_Comparison = copy_From.Depth_Comparison; Write_toDepthBuffer = copy_From.Write_toDepthBuffer;
		return *this;
	}

	bool operator!=(Stencil& compare) {
		return (!(compare == *this));
	}

	void Activate();
};

Stencil defaultStencilTest;
Stencil Stencil_MainPass(GL_GEQUAL, 0xFF, 0, 0xFF, GL_KEEP, GL_KEEP, GL_REPLACE, true, GL_LEQUAL, GL_TRUE);
Stencil Stencil_Skybox(GL_GREATER, 0xFF, 5, 0xFF, GL_KEEP, GL_KEEP, GL_REPLACE);
Stencil Stencil_PostProcess(GL_ALWAYS, 0xFF, 0, 0xFF, GL_KEEP, GL_KEEP, GL_REPLACE, false);
Stencil currentStencilTest;

void Stencil::Activate() {
	//Compare with Current Stencil Settings
	if (*this == currentStencilTest) {}

	//If Current Stencil Settings are different
	else {
		currentStencilTest = *this;

		//Enable the Stencil Test
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(Comparison_Way, Stencil_Value, Buffer_Value);
		glStencilOp(When_StencilFails, When_DepthFails, When_AllPasses);
		glStencilMask(Write_toBuffer);	//Write to the Buffer
		if (Depth_Test) {
			set_DepthTest(Depth_Comparison, Write_toDepthBuffer);
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}
	}

}




void set_FaceCulling(GLint WindingOrder_Type = GL_CCW, GLint Culling_Face = GL_BACK) {
	glEnable(GL_CULL_FACE);
	glFrontFace(WindingOrder_Type);
	glCullFace(Culling_Face);
}

void Test_FaceCulling() {
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		set_FaceCulling(GL_CCW, GL_BACK);
	}
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		set_FaceCulling(GL_CCW, GL_FRONT);
	}
	else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		set_FaceCulling(GL_CCW, GL_FRONT_AND_BACK);
	}
}




//1)If you call without parameter or true, it will set all Framebuffers as Wireframe or not;
//according to your Keyboard Input.
//If you call with false parameter, it will set the remaining Framebuffers as not Wireframe
void wireframe_Transition(bool renderloop_wireframe = true) {
	if (renderloop_wireframe == false) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		temporary_wireframe = true;
	}
	else {
		if (wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			if (wireframe) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				wireframe = false;
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				wireframe = true;
			}
		}
	}

}

void determine_Transform(GLFWwindow* window, mat4 &gecici, float rotate_vec = 0, vec3 translate_vec = vec3(0, 0, 0), vec3 scale_vec = vec3(1, 1, 1)) {
	if (rotate_vec == 0 && translate_vec == vec3(0, 0, 0) && scale_vec == vec3(1, 1, 1)) {
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			gecici = rotate(gecici, radians(5.0f), vec3(0, 0, 1));
			gecici = translate(gecici, vec3(-0.1f, 0, 0));
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			gecici = scale(gecici, vec3(1, 1.0f, 1));
			gecici = rotate(gecici, -(float)glfwGetTime(), vec3(0, 0, 1));
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			gecici = rotate(gecici, radians(-5.0f), vec3(0, 0, 1));
			gecici = translate(gecici, vec3(0.1f, 0, 0));
		}
		else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			gecici = scale(gecici, vec3(1, 1.0f, 1));
			gecici = rotate(gecici, (float)glfwGetTime(), vec3(0, 0, 1));
		}
		else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			mat4 reset;
			gecici = reset;
		}
	}
	else {
		gecici = scale(gecici, scale_vec);
		gecici = rotate(gecici, rotate_vec, vec3(0, 0, 1));
		gecici = translate(gecici, translate_vec);
	}
}



void Read_Vec3(const vec3 &input) {
	cout << "X: " << input.x << "\nY: " << input.y << "\nZ: " << input.z << endl;
}

