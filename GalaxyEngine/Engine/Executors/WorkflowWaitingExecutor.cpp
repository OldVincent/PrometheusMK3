#include "WorkflowWaitingExecutor.hpp"
#include "../Core/Tools/WorkflowAccess.hpp"

namespace Galaxy
{
	/// 提交
	void WorkflowWaitingExecutor::Submit(Core::AbstractWorkflow *workflow)
	{
		auto awaken_finder = AwakenWorkflows.find(workflow);
		if (awaken_finder != AwakenWorkflows.end())
		{
			Core::Tools::WorkflowAccess::IterateExecute(*awaken_finder);
			AwakenWorkflows.unsafe_erase(awaken_finder);
			return;
		}
		WaitingWorkflows.insert(workflow);
	}

	/// 唤醒
	void WorkflowWaitingExecutor::Awake(Core::AbstractWorkflow *workflow)
	{
		auto waiting_finder = WaitingWorkflows.find(workflow);
		if (waiting_finder != WaitingWorkflows.end())
		{
			auto* target_executor = Core::Tools::WorkflowAccess::GetCurrentExecutor(*waiting_finder);
			target_executor->Submit(*waiting_finder);
			WaitingWorkflows.unsafe_erase(waiting_finder);
			return;
		}
		AwakenWorkflows.insert(workflow);
	}
}

