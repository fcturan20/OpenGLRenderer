#version 330 core
layout (location = 0) in vec3 VertexPos;

out vec3 TextCoord;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main(){
    gl_Position = (projection_matrix * view_matrix * vec4(VertexPos, 1.0)).xyww;
	TextCoord = VertexPos;
}