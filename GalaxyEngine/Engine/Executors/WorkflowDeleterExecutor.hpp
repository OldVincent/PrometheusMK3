#pragma once

#include "../Core/AbstractExecutor.hpp"

namespace Galaxy
{
	/**
	 * @brief 工作流删除器
	 * @author Vincent
	 * @details
	 *  ~ 该执行器用于删除工作流。
	 *  ~ 提交的工作流首先会被调用OnEnd方法，随后会被删除。
	 *  ~ 工作流的删除方法将在分离的线程中执行。
	 */
	class WorkflowDeleterExecutor : public Core::AbstractExecutor
	{
	public:
	protected:
		void OnUpdateWorkingThread() override;

	public:
		/**
		 * @brief 提交方法
		 * @param workflow 需要被删除的工作流
		 * @details
		 */
		void Submit(Core::AbstractWorkflow *workflow) override;
	};
}