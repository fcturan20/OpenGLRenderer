#version 330 core
in vec3 TextCoord;

out vec4 Fragment_Color;

uniform samplerCube skybox;

void main(){
	Fragment_Color = texture(skybox, TextCoord);
}