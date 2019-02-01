#version 330 core
in vec2 TextCoords;

out vec4 Fragment_Color;

uniform sampler2D RenderResult;
uniform sampler2D DepthResult;

void main(){
	float gamma = 2.2;
	Fragment_Color = texture(RenderResult, TextCoords);
	//vec4 Fragment_Color = vec4(gl_FragCoord.z);
}