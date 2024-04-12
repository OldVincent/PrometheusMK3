#pragma once

#include "../Core/AbstractExecutor.hpp"
#include <tbb/tbb.h>

namespace Galaxy
{
	/**
	 * @brief 工作流等待表
	 * @author Vincent
	 * @details
	 *  ~ 工作流等待集合可以存储和唤醒工作流。
	 */
	class WorkflowWaitingExecutor : public Core::AbstractExecutor
	{
	private:
		/// 正在等待的工作流，存放已经挂起，但工作流尚未抵达的工作流指针
		tbb::concurrent_unordered_set<Core::AbstractWorkflow*> WaitingWorkflows;
		/// 需要唤醒的工作流，存放唤醒消息已经到达，但工作流尚未挂起的工作流指针
		tbb::concurrent_unordered_set<Core::AbstractWorkflow*> AwakenWorkflows;

	public:
		/// 不进行任何操作
		void Start() override
		{}
		/// 不进行任何操作
		void Stop() override
		{}
		/// 不进行任何操作
		void Join() override
		{}

		/// 判断是否正在工作，恒为true
		[[nodiscard]] bool IsWorking() const override
		{
			return true;
		}

		/**
		 * @brief 提交工作流
		 * @param workflow 工作流
		 */
		void Submit(Core::AbstractWorkflow *workflow) override;

		/**
		 * @brief 唤醒工作线程
		 * @details
		 *  ~ 将在调用线程中继续执行该工作流。
		 */
		virtual void Awake(Core::AbstractWorkflow *workflow);

	protected:
		/// 不进行任何操作
		void OnUpdateWorkingThread() override
		{}
	};
}