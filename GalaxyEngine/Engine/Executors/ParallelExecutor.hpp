#pragma once

#include "GalaxyEngine/Engine/Core/AbstractExecutor.hpp"

#include <list>
#include <mutex>
#include <atomic>

namespace Galaxy
{
	/**
	 * @brief 并行执行器
	 * @author Vincent
	 * @details
	 *  ~ 该执行器将并行地执行工作流。
	 *  ~ 注意，并行执行的工作线程默认将具备父线程的CPU亲和性。
	 */
	class ParallelExecutor : public Core::AbstractExecutor
	{
	public:
		using AbstractExecutor::AbstractExecutor;

	private:
		/// 工作任务队列
		std::list<Core::AbstractWorkflow*> WorkingTasks;
		/// 等候任务队列
		std::list<Core::AbstractWorkflow*> WaitingTasks;
		/// 等候队列互斥锁
		mutable std::mutex WaitingTasksMutex;

		/// 工作队列是否为空
		std::atomic_bool WorkingTasksEmpty {true};
		/// 等待队列是否为空
		std::atomic_bool WaitingTasksEmpty {true};

	public:
		/**
		 * @brief 提交工作流
		 * @param pipeline 工作流指针
		 * @details
		 *  ~ 由于并发处理器使用了交换链，故此处需要重载，以将等待执行的工作流提交到交换链中。
		 */
		void Submit(Core::AbstractWorkflow* workflow) override;

	protected:
		/// 更新事件
		void OnUpdateWorkingThread() override;

	public:
		/**
		 * @brief 返回等待任务列表是否为空
		 * @retval true 等待列表为空
		 * @retval false 等待列表非空
		 */
		bool IsEmpty() const override
		{
			return WaitingTasksEmpty && WorkingTasksEmpty;
		}
	};
}
