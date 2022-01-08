#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragWorldPos;
layout(location = 2) out vec3 fragWorldNormal;

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

void main()
{
// ������Ʒ�͹��λ�ö�û�о����κα任��Ĭ����������ͬһ����ϵ
	vec4 worldPos = push.modelMatrix * vec4(position, 1.0f);
	gl_Position = ubo.projectionMatrix * ubo.viewMatrix * push.modelMatrix * vec4(position, 1.0);

	fragColor = color;
	fragWorldPos = worldPos.xyz;
	fragWorldNormal = normalize(mat3(push.normalMatrix) * normal);
}