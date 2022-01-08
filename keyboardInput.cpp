#include "pch.h"
#include "keyboardInput.hpp"
#include <glm/gtc/constants.hpp>

void keyboardInput::moveInPlaneXZ(GLFWwindow* win, float dt, vulkanGameobject& obj)
{
	glm::vec3 rotate{ 0 };
	if (glfwGetKey(win, keys.right) == GLFW_PRESS) rotate.y += 1.0f;
	if (glfwGetKey(win, keys.left) == GLFW_PRESS) rotate.y -= 1.0f;
	if (glfwGetKey(win, keys.up) == GLFW_PRESS) rotate.x += 1.0f;
	if (glfwGetKey(win, keys.down) == GLFW_PRESS) rotate.x -= 1.0f;
	//if (glfwGetKey(win, keys.n) == GLFW_PRESS) rotate.z += 1.0f;
	//if (glfwGetKey(win, keys.m) == GLFW_PRESS) rotate.z -= 1.0f;

	if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
		obj.transform.rotation += rotSpeed * dt * glm::normalize(rotate);
	obj.transform.rotation.y = glm::mod(obj.transform.rotation.y, glm::two_pi<float>());
	obj.transform.rotation.x = glm::mod(obj.transform.rotation.x, glm::two_pi<float>());
	float yaw = obj.transform.rotation.y;
	const glm::vec3 forwardDir{ sin(yaw), 0, cos(yaw) };
	const glm::vec3 rightDir{ cos(yaw), 0, -sin(yaw) };
	const glm::vec3 upDir{ 0, -1, 0 };

	glm::vec3 moveDir{ 0 };
	if (glfwGetKey(win, keys.w) == GLFW_PRESS) moveDir += forwardDir;
	if (glfwGetKey(win, keys.s) == GLFW_PRESS) moveDir -= forwardDir;
	if (glfwGetKey(win, keys.a) == GLFW_PRESS) moveDir -= rightDir;
	if (glfwGetKey(win, keys.d) == GLFW_PRESS) moveDir += rightDir;
	if (glfwGetKey(win, keys.lCtrl) == GLFW_PRESS) moveDir -= upDir;
	if (glfwGetKey(win, keys.space) == GLFW_PRESS) moveDir += upDir;
	if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
		obj.transform.translation += movSpeed * dt * glm::normalize(moveDir);
}