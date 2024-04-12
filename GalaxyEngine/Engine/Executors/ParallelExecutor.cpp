#include "ParallelExecutor.hpp"

#include <tbb/tbb.h>

#include <utility>

namespace Galaxy
{
	/// 更新事件
	void ParallelExecutor::OnUpdateWorkingThread()
	{
		// 判断任务列表是否为空
		if (!this->WorkingTasks.empty())
		{
			tbb::parallel_for_each(this->WorkingTasks.begin(), this->WorkingTasks.end(),
					 [this](Core::AbstractWorkflow* workflow){
				// 执行并提交
				AbstractExecutor::InvokeWorkflow(workflow);
			});
			this->WorkingTasks.clear();
		}
		else
		{
			this->WorkingTasksEmpty = true;
			// 翻转
			std::unique_lock waiting_lock(this->WaitingTasksMutex);
			if (!this->WaitingTasks.empty())
			{
				this->WorkingTasks.swap(this->WaitingTasks);
				this->WorkingTasksEmpty = false;
				this->WaitingTasksEmpty = true;
			}
			waiting_lock.unlock();

			// 交换后依然为空
			if (this->WorkingTasks.empty())
			{
				// 为空，则交出一次当前的时间片
				std::this_thread::yield();
			}
		}
	}

	/// 提交工作流
	void ParallelExecutor::Submit(Core::AbstractWorkflow* workflow)
	{
		std::unique_lock lock(WaitingTasksMutex);
		WaitingTasks.push_back(workflow);
		WaitingTasksEmpty = false;
	}
}
