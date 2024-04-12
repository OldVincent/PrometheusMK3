#include "WaitAction.hpp"

#include "../Runtime.hpp"

namespace Galaxy::BuiltIn
{
	/// 执行方法
	void WaitAction::Execute()
	{
		Runtime::GetInstance()->WorkflowWaitingZone.Submit(GetWorkflow());
	}
}
