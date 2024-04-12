#include "AwakeAction.hpp"
#include "../Runtime.hpp"

namespace Galaxy::BuiltIn
{
	/// 执行方法
	void AwakeAction::Execute()
	{
		Runtime::GetInstance()->WorkflowWaitingZone.Awake(*TargetWorkflow);
	}
}
