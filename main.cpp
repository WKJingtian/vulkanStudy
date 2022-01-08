// 李景天 vulkan 学习用代码
#include "pch.h"
#include "vulkanApp.hpp"

/*
一些规则：
	所有类的所有变量都是公开的
	命名使用首字母小写的驼峰
	缩进用TAB（虽然有时候VS会强行换成空格）
	使用VS自带的build系统，所有的依赖都放在根目录（和代码一起），或者vcpkg下
	所有shader相关都在shaders文件夹（包括用于编译的命令行）
目标：
	整合IMGUI，制造一个可以支持简单操作的前端
	支持模型导入，制造一个漂亮的场景
资源：
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