#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragWorldPos;
layout(location = 2) in vec3 fragWorldNormal;
//layout (location = 3) in vec3 inNormal;
//layout (location = 4) in vec3 inView;
//layout (location = 5) in vec3 inLight;

layout (location = 0) out vec4 outColor;

struct pointLight
{
	vec4 pos;
	vec4 col;
};

layout(set = 0, binding = 0) uniform globalUBO
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 ambientLight;
	vec4 camPos;
	pointLight lights[10];
	int numLight;
} ubo;

layout(push_constant) uniform Push
{
	mat4 modelMatrix;
	mat4 normalMatrix;
} push;

float PHONE_EXPONENTIAL = 40.0f;

void main()
{
	vec3 diffuseLight = ubo.ambientLight.xyz * ubo.ambientLight.w;
	vec3 surfaceNormal = normalize(fragWorldNormal);
	vec3 directionToView = ubo.viewMatrix[3].xyz - fragWorldPos;
	for(int i = 0; i < ubo.numLight; i++)
	{
		pointLight light = ubo.lights[i];
		vec3 directionToLight = light.pos.xyz - fragWorldPos;
		float attenuation = 0.8f / dot(directionToLight, directionToLight);
		float cosAngle = max(dot(surfaceNormal, normalize(directionToLight)), 0);
		vec3 intensity = light.col.xyz * light.col.w * attenuation;
		diffuseLight += intensity * cosAngle;
		
		vec3 N = surfaceNormal;
		vec3 L = normalize(directionToLight) * -1;
		vec3 V = normalize(directionToView);
		vec3 R = reflect(-L, N);
		vec3 specular = light.col.w * light.col.xyz * pow(max(dot(R, V), 0.0), PHONE_EXPONENTIAL);
		diffuseLight += specular;
	}
    
	outColor = vec4(diffuseLight * fragColor, 1.0);
}
