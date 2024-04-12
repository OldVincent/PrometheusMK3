#pragma once

#include "../Engine/Core/AbstractProcessor.hpp"
#include <type_traits>

namespace Galaxy
{
	/**
	 * @brief 处理器类
	 * @author Vincent
	 * @details
	 *  ~ 供用户实现并派生的处理器类。
	 */
	class Processor : public Core::AbstractProcessor
	{
	public:
		using AbstractProcessor::AbstractProcessor;

		/**
		 * @brief 构造函数
		 * @tparam ExecutorType 执行器类型
		 * @tparam WorkflowType 工作流类型
		 * @param executor 指向执行器指针的指针
		 * @param workflow 工作流指针
		 */
		template<typename ExecutorType, typename WorkflowType,
				typename = typename std::enable_if<std::is_base_of_v<Core::AbstractExecutor, ExecutorType>>,
				typename = typename std::enable_if<std::is_base_of_v<Core::AbstractWorkflow, WorkflowType>>>
		Processor(ExecutorType** executor, WorkflowType* workflow) :
			AbstractProcessor((Core::AbstractExecutor**)(executor), (Core::AbstractWorkflow*)(workflow))
		{}
	};
}
