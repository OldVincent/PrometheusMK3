#pragma once

#include "../Core/AbstractProcessor.hpp"

#include <functional>

namespace Galaxy::BuiltIn
{
	/**
	 * @brief Lambda流处理器
	 * @author Vincent
	 * @details
	 *  ~ 该流处理器将会执行Lambda表达式的内容。
	 */
	class LambdaAction : public Core::AbstractProcessor
	{
	public:
		using Core::AbstractProcessor::AbstractProcessor;

		/// 流处理器将要执行的函数器
		std::function<void()> Operation;

	public:
		/**
		 * @brief 构造函数
		 * @param target_executor 目标执行器
		 * @param host 宿主
		 * @param operation 操作
		 */
		template<typename ExecutorType, typename WorkflowType,
				typename = typename std::enable_if<std::is_base_of_v<Core::AbstractExecutor, ExecutorType>>,
				typename = typename std::enable_if<std::is_base_of_v<Core::AbstractWorkflow, WorkflowType>>>
		LambdaAction(ExecutorType** target_executor, WorkflowType* host, decltype(Operation) operation) :
			Core::AbstractProcessor((Core::AbstractExecutor**)(target_executor), (Core::AbstractWorkflow*)(host)),
			Operation(std::move(operation))
		{}

	protected:
		/// 执行方法
		void Execute() override;
	};
}
