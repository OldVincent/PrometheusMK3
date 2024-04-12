#pragma once

#include "../Engine/Core/AbstractWorkflow.hpp"

namespace Galaxy
{
	/**
	 * @brief 工作流
	 * @author Vincent
	 * @details
	 *  ~ 工作流用于描述和承载通道、处理器顺序。
	 *  ~ 工作流内可以定义执行器指针、通道和流处理器。
	 *  ~ 执行器指针必须在该工作流被提交给某个引擎前就赋值。
	 *  ~ 其内定义的通道将按照名称被流处理器自动挂载，若存在流处理器找不到对应通道，则将抛出异常。
	 *  ~ 其内的流处理器将按照定义顺序在指定的执行器上执行，注意，只有上一个流处理器完成后才会执行下一个流处理器。
	 */
	class Workflow : public Core::AbstractWorkflow
	{

	};
}