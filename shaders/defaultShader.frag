#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragWorldPos;
layout (location = 2) in vec3 fragWorldNormal;

layout (location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform globalUBO
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 ambientLight;
	vec4 lightPos;
	vec4 lightColor;
} ubo;

layout(push_constant) uniform Push
{
	mat4 modelMatrix;
	mat4 normalMatrix;
} push;

void main() {
	vec3 directionToLight = ubo.lightPos.xyz - fragWorldPos;
	float attenuation = 1.0f / dot(directionToLight, directionToLight);
	vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
	vec3 ambientColor = ubo.ambientLight.xyz * ubo.ambientLight.w;
	vec3 diffuseLight = lightColor * max(dot(normalize(fragWorldNormal),
						normalize(directionToLight)), 0);
	outColor = vec4((ambientColor + diffuseLight) * fragColor, 1.0);
}
