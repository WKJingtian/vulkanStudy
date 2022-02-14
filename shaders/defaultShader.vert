#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragWorldPos;
layout(location = 2) out vec3 fragWorldNormal;
//layout (location = 3) out vec3 outNormal;
//layout (location = 4) out vec3 outView;
//layout (location = 5) out vec3 outLight;

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

void main()
{
// 这里物品和光的位置都没有经过任何变换，默认他们是在同一坐标系
	vec4 worldPos = push.modelMatrix * vec4(position, 1.0f);
	gl_Position = ubo.projectionMatrix * ubo.viewMatrix * push.modelMatrix * vec4(position, 1.0);

	fragColor = color;
	fragWorldPos = worldPos.xyz;
	fragWorldNormal = normalize(mat3(push.normalMatrix) * normal);
	
	//test
    //outNormal =  mat3(push.normalMatrix) * normal;
    //outLight =  ubo.lightPos.xyz - worldPos.xyz;
    //outView = ubo.viewMatrix[3].xyz - worldPos.xyz;
}
