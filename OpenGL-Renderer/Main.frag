#version 330 core
in vec2 TextCoord;
in vec3 VertexNormal;
in vec3 FragmentPos;
in mat3 TBN;
out vec4 Fragment_Color;

//Light Struct and Light Input/Uniform
struct PointLight{
	vec4 Position;
	vec4 Color;
	vec4 Constant;
	vec4 Linear;
	vec4 Quadratic;
};

layout (std140) uniform Lights_Point{
	PointLight point_lights[500];
};

struct DirectionalLight{
	vec3 Position;
	vec3 Color;
	vec3 Direction;
	float Constant;
	float Linear;
	float Quadratic;
	int Attenuation;
};

struct SpotLight{
	vec3 Position;
	vec3 Color;
	vec3 Direction;
	float InnerCutOffCos;
	float OuterCutOffCos;
	float Constant;
	float Linear;
	float Quadratic;
	int Attenuation;
};


//Material Struct and Material Spec Input/Uniform
struct MaterialSpecs{
	vec3 Ambient_Factor;
	vec3 Diffuse_Factor;
	vec3 Specular_Factor;
	float Shininess;
};

uniform MaterialSpecs material;

//Forward Declarations
vec3 ambient_Lighting(vec3 f_Color);
vec3 emission_Lighting(sampler2D f_emission_map);
float Calculate_Attenuation(vec3 lightPos, float Constant, float Linear, float Quadratic);


vec3 Calculate_Point_Lights();
//vec3 Calculate_Directional_Lights();
//vec3 Calculate_Spot_Lights();




//Input Parameters to Color the Object
uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;
uniform sampler2D texture_emission0;
uniform samplerCube CubeMap;
uniform float mixture_level = 0.5f;
uniform vec3 view_Pos = vec3(0);
uniform float TextCoordSampler = 1;


//Linearize the Depth
uniform float frustum_near = 0.1f;
uniform float frustum_far = 100.0f;

float LinearizeDepth(){
	float clipspace_z = (gl_FragCoord.z * 2) - 1;
	return ((2 * frustum_near * frustum_far) / ((frustum_far + frustum_near) - (clipspace_z * (frustum_far - frustum_near)))) / frustum_far;
}



void main() {
	//Main Object Color
	vec4 objectColor = texture(texture_diffuse0, TextCoord * TextCoordSampler);


	

	vec4 Lighting_Calculations = vec4(Calculate_Point_Lights(),1.0f);
	float depth = LinearizeDepth();
	gl_FragDepth = depth;

	//Reflection Calculation
	vec3 eyeToFragment = normalize(FragmentPos - view_Pos);
	vec3 reflectDirection = reflect(eyeToFragment, normalize(VertexNormal));

	//Refraction Calculation
	float ratio = 1.0f/1.52f;
	vec3 refractDirection = refract(eyeToFragment, normalize(VertexNormal), ratio);

	
		vec3 normal = normalize(VertexNormal);
	//Final Fragment Color
	//Fragment_Color = vec4(normal, 1.0f);
	Fragment_Color = vec4(normal, 1.0f);
	//Fragment_Color = vec4(depth);

}



	//Ambient Lighting
vec3 ambient_Lighting(vec3 f_Color){
	return f_Color * material.Ambient_Factor;
}


vec3 emission_Lighting(sampler2D f_emission_map){
	return vec3(texture(f_emission_map, TextCoord * TextCoordSampler));
}

float Calculate_Attenuation(vec3 lightPos, float Constant, float Linear, float Quadratic){
	float Distance = length(lightPos - FragmentPos);
	return 1 / (Constant + (Distance * Linear) + (Quadratic * (Distance * Distance)));
}


	//Calculate Point Lights and Add All Results to A Vec3 (Done Correctly)
vec3 Calculate_Point_Lights(){
	vec3 result = vec3(0);
	for(int i = int(0); i < int(2); i++) {
		PointLight light = point_lights[i];

		//Ambient Lighting
		vec3 Ambient_Light = ambient_Lighting(vec3(light.Color));

		//Diffuse Lighting
		//vec3 normal = texture(texture_normal0, TextCoord * TextCoordSampler).rgb;
		//normal = (normal * 2) - 1;
		//normal = normalize(TBN * normal);
		vec3 normal = normalize(VertexNormal);

		vec3 fragmentToLightDir = normalize(vec3(light.Position) - FragmentPos);
		float diff_strength = max(dot(normal, fragmentToLightDir), 0.0f);
		vec3 Diffuse_Light = diff_strength * vec3(light.Color) * material.Diffuse_Factor;

		//Specular Lighting
		vec3 viewDir = normalize(view_Pos - FragmentPos);
		vec3 reflectDir = reflect(viewDir, fragmentToLightDir);
		vec3 halfwayDir = normalize(fragmentToLightDir + viewDir);

		float specularity = pow(max(dot(viewDir, halfwayDir),0.0), material.Shininess);
		vec3 Specular_Light = specularity * vec3(light.Color) * vec3(texture(texture_specular0, TextCoord * TextCoordSampler));
		//Result per Light
		//float Attenuation = Calculate_Attenuation(vec3(light.Position), float(1), float(light.Linear), float(light.Quadratic));
		float Attenuation = 1;
		result += (Ambient_Light + Diffuse_Light + Specular_Light) * Attenuation;
	}
	return result;
}