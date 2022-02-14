#version 450

const vec2 OFFSETS[6] = vec2[]
(
	vec2(-1.0, -1.0),
	vec2(-1.0, 1.0),
	vec2(1.0, -1.0),
	vec2(1.0, -1.0),
	vec2(-1.0, 1.0),
	vec2(1.0, 1.0)
);
layout(location = 0) out vec2 fragOffset;

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
layout (push_constant) uniform Push
{
	vec4 pos;
	vec4 col;
	float rad;
} push;
void main()
{
	fragOffset = OFFSETS[gl_VertexIndex];
	vec3 cameraRightWorld = {ubo.viewMatrix[0][0], ubo.viewMatrix[1][0], ubo.viewMatrix[2][0]};
	vec3 cameraUpWorld = {ubo.viewMatrix[0][1], ubo.viewMatrix[1][1], ubo.viewMatrix[2][1]};
	vec3 positionWorld = push.pos.xyz
		+ push.rad * fragOffset.x * cameraRightWorld
		+ push.rad * fragOffset.y * cameraUpWorld;
	gl_Position = ubo.projectionMatrix * ubo.viewMatrix * vec4(positionWorld, 1.0);
}
