#pragma once

#include <initializer_list>
#include <string>
#include <vector>
#include <tuple>

namespace Galaxy::Core
{
	/// 抽象管道类
	class AbstractWorkflow;

	/**
	 * @brief 抽象通道类
	 * @author Vincent
	 * @details
	 *  ~ 抽象通道类提供数据对象管理和端口挂载服务。
	 */
	class AbstractChannel
	{
	public:
		/**
		 * @brief 构造函数
		 * @param host 宿主工作流指针
		 * @param names 名称列表，这些名称都将指向该通道类
		 */
		AbstractChannel(AbstractWorkflow *host, const std::initializer_list<const char*>& names);

		/**
		 * @brief 构造函数
		 * @param name 名称
		 * @param host 宿主通道指针
		 */
		AbstractChannel(AbstractWorkflow* host, const char* name);
	};
}