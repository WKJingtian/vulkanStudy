#include "pch.h"
#include "vulkanCamera.hpp"

void vulkanCamera::setOrthographicStats(float l, float r, float t, float b, float n, float f)
{
	projectionMatrix = glm::mat4{ 1.0f };
	projectionMatrix[0][0] = 2.f / (r - l);
	projectionMatrix[1][1] = 2.f / (b - t);
	projectionMatrix[2][2] = 1.f / (f - n);
	projectionMatrix[3][0] = -(r + l) / (r - l);
	projectionMatrix[3][1] = -(b + t) / (b - t);
	projectionMatrix[3][2] = -n / (f - n);
	isOrthographic = true;
}

void vulkanCamera::setPerspectiveStats(float fov, float asp, float n, float f)
{
	assert(glm::abs(asp - std::numeric_limits<float>::epsilon()) > 0.0f);
	const float tanHalfFovy = tan(fov / 2.f);
	projectionMatrix = glm::mat4{ 0.0f };
	projectionMatrix[0][0] = 1.f / (asp * tanHalfFovy);
	projectionMatrix[1][1] = 1.f / (tanHalfFovy);
	projectionMatrix[2][2] = f / (f - n);
	projectionMatrix[2][3] = 1.f;
	projectionMatrix[3][2] = -(f * n) / (f - n);
	isOrthographic = false;
}
void vulkanCamera::setViewDirection(glm::vec3 pos, glm::vec3 direc, glm::vec3 up)
{
	const glm::vec3 w{ glm::normalize(direc) };
	const glm::vec3 u{ glm::normalize(glm::cross(w, up)) };
	const glm::vec3 v{ glm::cross(w, u) };

	viewMatrix = glm::mat4{ 1.f };
	viewMatrix[0][0] = u.x;
	viewMatrix[1][0] = u.y;
	viewMatrix[2][0] = u.z;
	viewMatrix[0][1] = v.x;
	viewMatrix[1][1] = v.y;
	viewMatrix[2][1] = v.z;
	viewMatrix[0][2] = w.x;
	viewMatrix[1][2] = w.y;
	viewMatrix[2][2] = w.z;
	viewMatrix[3][0] = -glm::dot(u, pos);
	viewMatrix[3][1] = -glm::dot(v, pos);
	viewMatrix[3][2] = -glm::dot(w, pos);
	camPos = pos;
}
void vulkanCamera::setViewTarget(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
{
	assert(pos != target && "camera: target cannot be equal to position!");
	setViewDirection(pos, target - pos, up);
}
void vulkanCamera::setViewXYZ(glm::vec3 pos, glm::vec3 rot)
{
	const float c3 = glm::cos(rot.z);
	const float s3 = glm::sin(rot.z);
	const float c2 = glm::cos(rot.x);
	const float s2 = glm::sin(rot.x);
	const float c1 = glm::cos(rot.y);
	const float s1 = glm::sin(rot.y);
	const glm::vec3 u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
	const glm::vec3 v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
	const glm::vec3 w{ (c2 * s1), (-s2), (c1 * c2) };
	viewMatrix = glm::mat4{ 1.f };
	viewMatrix[0][0] = u.x;
	viewMatrix[1][0] = u.y;
	viewMatrix[2][0] = u.z;
	viewMatrix[0][1] = v.x;
	viewMatrix[1][1] = v.y;
	viewMatrix[2][1] = v.z;
	viewMatrix[0][2] = w.x;
	viewMatrix[1][2] = w.y;
	viewMatrix[2][2] = w.z;
	//viewMatrix[3][0] = pos.x;
	//viewMatrix[3][1] = pos.y;
	//viewMatrix[3][2] = pos.z;
	viewMatrix[3][0] = -glm::dot(u, pos);
	viewMatrix[3][1] = -glm::dot(v, pos);
	viewMatrix[3][2] = -glm::dot(w, pos);
	camPos = pos;
}