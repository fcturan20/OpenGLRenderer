#version 330 core
in vec2 TextCoords;

out vec4 Fragment_Color;

uniform sampler2D RenderResult;
uniform sampler2D DepthResult;

void main(){
	float gamma = 2.2;
	vec3 RendererInput = pow(texture(RenderResult, TextCoords).rgb, vec3(1/gamma));
	//vec3 RendererInput = vec3(gl_FragCoord.z);
	Fragment_Color = vec4(RendererInput, 1.0f);
}