#pragma once
#include "vulkanGameobject.hpp"

class keyboardInput
{
public:
	struct keyMapping
	{
		int a = GLFW_KEY_A;
		int b = GLFW_KEY_B;
		int c = GLFW_KEY_C;
		int d = GLFW_KEY_D;
		int e = GLFW_KEY_E;
		int f = GLFW_KEY_F;
		int g = GLFW_KEY_G;
		int h = GLFW_KEY_H;
		int i = GLFW_KEY_I;
		int j = GLFW_KEY_J;
		int k = GLFW_KEY_K;
		int l = GLFW_KEY_L;
		int m = GLFW_KEY_M;
		int n = GLFW_KEY_N;
		int o = GLFW_KEY_O;
		int p = GLFW_KEY_P;
		int q = GLFW_KEY_Q;
		int r = GLFW_KEY_R;
		int s = GLFW_KEY_S;
		int t = GLFW_KEY_T;
		int u = GLFW_KEY_U;
		int v = GLFW_KEY_V;
		int w = GLFW_KEY_W;
		int x = GLFW_KEY_X;
		int y = GLFW_KEY_Y;
		int z = GLFW_KEY_Z;
		int n0 = GLFW_KEY_0;
		int n1 = GLFW_KEY_1;
		int n2 = GLFW_KEY_2;
		int n3 = GLFW_KEY_3;
		int n4 = GLFW_KEY_4;
		int n5 = GLFW_KEY_5;
		int n6 = GLFW_KEY_6;
		int n7 = GLFW_KEY_7;
		int n8 = GLFW_KEY_8;
		int n9 = GLFW_KEY_9;
		int left = GLFW_KEY_LEFT;
		int right = GLFW_KEY_RIGHT;
		int up = GLFW_KEY_UP;
		int down = GLFW_KEY_DOWN;
		int space = GLFW_KEY_SPACE;
		int lCtrl = GLFW_KEY_LEFT_CONTROL;
		int lShift = GLFW_KEY_LEFT_SHIFT;
	};

	keyMapping keys{};
	float movSpeed = 0.5f;
	float rotSpeed = 2.0f;

	void moveInPlaneXZ(GLFWwindow* win, float dt, vulkanGameobject& obj);
};