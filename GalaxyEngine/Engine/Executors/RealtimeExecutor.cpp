#include "RealtimeExecutor.hpp"
#include "../Core/AbstractExecutor.hpp"

namespace Galaxy
{
	/// 提交工作流
	void RealtimeExecutor::Submit(Core::AbstractWorkflow *workflow)
	{
		InvokeWorkflow(workflow);
	}

	/// 更新工作线程
	void RealtimeExecutor::OnUpdateWorkingThread()
	{}
}