// ��� vulkan ѧϰ�ô���
#include "pch.h"
#include "vulkanApp.hpp"

/*
һЩ����
	����������б������ǹ�����
	����ʹ������ĸСд���շ�
	������TAB����Ȼ��ʱ��VS��ǿ�л��ɿո�
	ʹ��VS�Դ���buildϵͳ�����е����������ڸ�Ŀ¼���ʹ���һ�𣩣�����vcpkg��
	����shader��ض���shaders�ļ��У��������ڱ���������У�
Ŀ�꣺
	����IMGUI������һ������֧�ּ򵥲�����ǰ��
	֧��ģ�͵��룬����һ��Ư���ĳ���
��Դ��
	https://github.com/blurrypiano/littleVulkanEngine/tree/master/littleVulkanEngine
	https://vulkan-tutorial.com/
*/

int main()
{
	try
	{
		vulkanApp app{};
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}