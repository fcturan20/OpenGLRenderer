#version 330 core
out vec4 Fragment_Color;

uniform vec3 light_Color;

//Input Parameters to Linearize the Depth
uniform float frustum_near = 0.1f;
uniform float frustum_far = 1000.0f;

void main() {

	float clipspace_z = (gl_FragCoord.z * 2) - 1;
	float linear_depth = (2 * frustum_near * frustum_far) / ((frustum_far + frustum_near - clipspace_z) * (frustum_far - frustum_near));

	Fragment_Color = vec4(light_Color , 1.0f);
}
