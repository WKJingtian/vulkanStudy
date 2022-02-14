#version 450
layout (location = 0) in vec2 fragOffset;
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
layout (push_constant) uniform Push
{
	vec4 pos;
	vec4 col;
	float rad;
} push;

void main()
{
    float dis = sqrt(dot(fragOffset, fragOffset));
    if (dis >= 1)
    {
        discard;
    }
    outColor = vec4(push.col.xyz, 1.0 - dis);
}