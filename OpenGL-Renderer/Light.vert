#version 330 core
layout (location = 0) in vec3 vertexPos;

uniform mat4 world_transform;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main()
{
    gl_Position = projection_matrix * view_matrix * world_transform * vec4(vertexPos, 1.0);
}