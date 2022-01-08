#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

class vulkanCamera
{
public:
	bool isOrthographic;
	void setOrthographicStats(float l, float r, float t, float b, float n, float f);
	void setPerspectiveStats(float fov, float asp, float n, float f);
	void setViewDirection(glm::vec3 pos, glm::vec3 direc, glm::vec3 up = glm::vec3(0, -1, 0));
	void setViewTarget(glm::vec3 pos, glm::vec3 target, glm::vec3 up = glm::vec3(0, -1, 0));
	void setViewXYZ(glm::vec3 pos, glm::vec3 rot);

	glm::mat4 projectionMatrix{ 1 };
	glm::mat4 viewMatrix{ 1 };
};