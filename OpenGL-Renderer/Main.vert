#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TextureCoord;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;
layout (location = 5) in vec3 Color;

out vec2 TextCoord;
out vec3 VertexNormal;
out vec3 FragmentPos;
out mat3 TBN;

uniform mat4 world_transform;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;


void main()
{
    gl_Position = projection_matrix * view_matrix * world_transform * vec4(Position, 1.0);
	TextCoord = TextureCoord;

	//Vertex Normal Calculation
	VertexNormal = normalize(vec3(world_transform * vec4(Normal, 0.0)));

	//Vertex Tangent Calculation
	vec3 VertexTangent = normalize(vec3(world_transform * vec4(Tangent, 0.0)));
	vec3 VertexBitanget = normalize(vec3(world_transform * vec4(Bitangent, 0.0)));
	//VertexTangent = normalize(VertexTangent - dot(VertexNormal, VertexTangent) * VertexNormal);
	if(dot(cross(VertexTangent,VertexNormal), VertexBitanget) > 0){
		VertexTangent *= -1;
	}
	TBN = mat3(VertexTangent, VertexBitanget, VertexNormal);

	//Fragment Position
	FragmentPos = vec3(world_transform * vec4(Position, 1.0));
}
