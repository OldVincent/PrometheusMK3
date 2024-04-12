#include "WorkflowDeleterExecutor.hpp"
#include "../Core/AbstractWorkflow.hpp"
#include <thread>

namespace Galaxy
{
	/// 删除方法
	void WorkflowDeleterExecutor::Submit(Galaxy::Core::AbstractWorkflow *workflow)
	{
		delete workflow;
	}

	/// 更新方法，不进行任何操作
	void WorkflowDeleterExecutor::OnUpdateWorkingThread()
	{}
}


