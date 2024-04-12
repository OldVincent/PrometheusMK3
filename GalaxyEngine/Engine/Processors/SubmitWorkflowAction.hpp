#pragma once

#include <type_traits>
#include <functional>
#include <utility>
#include "../../Framework/Port.hpp"
#include "../../Framework/Processor.hpp"
#include "../../Framework/Channel.hpp"
#include "../../Engine/Core/AbstractExecutor.hpp"
#include "../../Engine/Core/Tools/WorkflowAccess.hpp"
#include "../Runtime.hpp"

namespace Galaxy::BuiltIn
{
	/**
	 * @brief 工作流提交动作
	 * @author Vincent
	 * @details
	 *  ~ 该动作将一个内置工作流提交到自身被指定的流处理器。
	 */
	template<typename WorkflowClass,
	        typename = typename std::enable_if<std::is_base_of_v<Core::AbstractWorkflow, WorkflowClass>>>
	class SubmitWorkflowAction : public Core::AbstractProcessor
	{
	public:
		/// 子工作流
		WorkflowClass SubWorkflow;

		/**
		 * @brief 是否同步地执行子工作流
		 * @details
		 *  ~ 默认为false，需要在构造函数中指定。
		 *  ~ 非同步工作模式下，父工作流将会进入等待区。
		 */
		bool Async;

		/**
		 * @brief 构造函数
		 * @param target_executor 目标执行器
		 * @param host 宿主
		 * @param workflow 需要提交的工作流
		 */
		template<typename ExecutorType, typename WorkflowType,
				typename = typename std::enable_if<std::is_base_of_v<Core::AbstractExecutor, ExecutorType>>,
				typename = typename std::enable_if<std::is_base_of_v<Core::AbstractWorkflow, WorkflowType>>>
		SubmitWorkflowAction(ExecutorType** target_executor, WorkflowType* host,
					   bool async = false) :
			Core::AbstractProcessor((Core::AbstractExecutor**)(target_executor), (Core::AbstractWorkflow*)(host)),
			Async(async)
		{
			if (!Async)
			{
				PauseWorkflowFlag = true;
				// 在结束事件中唤醒父工作流
				SubWorkflow.OnEnd = [workflow = this->GetWorkflow()]{
					Runtime::GetInstance()->WorkflowWaitingZone.Awake(workflow);
				};
			}
		}

	protected:
		/// 执行操作
		void Execute() override
		{
			(*GetExecutor())->Submit(&SubWorkflow);

			if (!Async)
			{
				Runtime::GetInstance()->WorkflowWaitingZone.Submit(GetWorkflow());
			}
		}
	};
}