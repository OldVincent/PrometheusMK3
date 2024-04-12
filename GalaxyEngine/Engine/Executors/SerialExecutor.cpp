#include "SerialExecutor.hpp"

#include <thread>

namespace Galaxy
{
	/// 更新事件
	void SerialExecutor::OnUpdateWorkingThread()
	{
		// 判断任务列表是否为空
		if (!this->Tasks.empty())
		{
			while(!this->Tasks.empty())
			{
				Core::AbstractWorkflow* workflow {nullptr};
				if (this->Tasks.try_pop(workflow))
				{
					AbstractExecutor::InvokeWorkflow(workflow);
				}
			}
		}
		else
		{
			// 为空，则交出一次当前的时间片
			std::this_thread::yield();
		}
	}
}
