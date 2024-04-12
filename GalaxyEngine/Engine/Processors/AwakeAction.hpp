#pragma once

#include <type_traits>

#include "../../Framework/Port.hpp"
#include "../../Framework/Processor.hpp"
#include "../../Framework/Channel.hpp"
#include "../../Engine/Core/AbstractExecutor.hpp"
#include "../../Engine/Core/Tools/WorkflowAccess.hpp"

namespace Galaxy::BuiltIn
{
	/**
	 * @brief 唤醒动作
	 * @author Vincent
	 * @details
	 *  ~ 该动作会将目标工作流从等待区唤醒，若目标工作路尚未进入等待区，则其下次进入等待区时将立即返回。
	 */
	class AwakeAction : public Core::AbstractProcessor
	{
	protected:
		/// 目标工作流
		Core::AbstractWorkflow** TargetWorkflow {nullptr};

	public:
		/**
		 * @brief 构造函数
		 * @param executor 目标执行器
		 * @param host 宿主
		 * @param target 需要唤醒的目标执行器
		 */
		template<typename ExecutorType, typename WorkflowType, typename TargetWorkflowType,
				typename = typename std::enable_if<std::is_base_of_v<Core::AbstractExecutor, ExecutorType>>,
				typename = typename std::enable_if<std::is_base_of_v<Core::AbstractWorkflow, WorkflowType>>,
				typename = typename std::enable_if<std::is_base_of_v<Core::AbstractWorkflow, TargetWorkflowType>>>
		AwakeAction(ExecutorType** executor, WorkflowType* host, TargetWorkflowType* target) :
				Core::AbstractProcessor((Core::AbstractExecutor**)(executor), (Core::AbstractWorkflow*)(host)),
				TargetWorkflow((Core::AbstractWorkflow**)(target))
		{}

	protected:
		/// 执行操作
		void Execute() override;
	};
}