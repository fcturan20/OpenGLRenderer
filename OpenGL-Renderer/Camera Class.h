#pragma once


class Camera {
public:
	vec3 Position, Front_Vector, Right_Vector, Up_Vector, target;
	mat4 view_matrix;
	void camera_Movement();
	void camera_Direction();
	void camera_Position();
	float cameraSpeed_Multiplier = 150.0f;
	float cameraSpeed_Base = 1.0f;
	void set_Speed(float speed);

	void Say_CamPos() {
		cout << "Camera Position:\n";
		Read_Vec3(Position);
	}

	Camera() : Position(0, 0, 0), target(0, 0, 1) {
		Front_Vector = target;
		camera_Movement();
	}

	Camera(vec3 Position) : Position(Position), target(0, 0, 1){
		Front_Vector = target;
		camera_Movement();
	}

	Camera(vec3 Position, vec3 target) : Position(Position), target(target) {
		Front_Vector = normalize(target - Position);
		camera_Movement();
	}
};

void Camera::camera_Direction() {
	if (mouse_yaw == 0 && mouse_pitch == 0) {
		return;
	}
	else {
		target.x = cos(radians(mouse_pitch)) * cos(radians(mouse_yaw));
		target.y = sin(radians(mouse_pitch));
		target.z = cos(radians(mouse_pitch)) * sin(radians(mouse_yaw));
	}
}

void Camera::set_Speed(float speed) {
	if (speed > 0) {
		cameraSpeed_Base = speed;
		cout << "Camera Speed is changed\n";
	}
	else {
		cout << "Changes are not applied, You set wrong Speed Number\n";
	}
}

void Camera::camera_Movement() {
	//Camera Rotation
	vec3 world_up(0, 1, 0);
	camera_Direction();
	Front_Vector = normalize(target);
	Right_Vector = normalize(cross(world_up, Front_Vector));
	Up_Vector = normalize(cross(Front_Vector, Right_Vector));

	//Camera Position
	camera_Position();


	mat4 view;
	view_matrix = view;
	view_matrix = lookAt(Position, Front_Vector + Position, world_up);
}


void Camera::camera_Position() {
	float camera_speed = cameraSpeed_Multiplier * Delta_Time * cameraSpeed_Base;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		Position += Front_Vector * camera_speed;

	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		Position -= Front_Vector * camera_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		Position -= Right_Vector * camera_speed;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		Position += Right_Vector * camera_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) {
		Position += Up_Vector * camera_speed;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
		Position -= Up_Vector * camera_speed;
	}
}

void camera_Mode(Camera *Camera, bool &firstMouse) {
	int mouse_right = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (firstMouse && mouse_right == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		Camera->camera_Movement();
		firstMouse = false;
	}
	if (mouse_right == GLFW_PRESS && !firstMouse) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		Camera->camera_Movement();
	}
	if (mouse_right != GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = true;
	}
}

Camera *main_Camera;