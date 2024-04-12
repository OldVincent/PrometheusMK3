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
	 * @brief 等待动作
	 * @author Vincent
	 * @details
	 *  ~ 该动作会将自身传入工作流等待区，等待唤醒。
	 */
	class WaitAction : public Core::AbstractProcessor
	{
	public:
		/**
		 * @brief 构造函数
		 * @param target_executor 目标执行器
		 * @param host 宿主
		 */
		template<typename ExecutorType, typename WorkflowType,
				typename = typename std::enable_if<std::is_base_of_v<Core::AbstractExecutor, ExecutorType>>,
				typename = typename std::enable_if<std::is_base_of_v<Core::AbstractWorkflow, WorkflowType>>>
		WaitAction(ExecutorType** target_executor, WorkflowType* host) :
				Core::AbstractProcessor((Core::AbstractExecutor**)(target_executor), (Core::AbstractWorkflow*)(host))
		{
			PauseWorkflowFlag = true;
		}

	protected:
		/// 执行操作
		void Execute() override;
	};
}