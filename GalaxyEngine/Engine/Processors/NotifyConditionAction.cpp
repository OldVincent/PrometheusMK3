#include "NotifyConditionAction.hpp"

namespace Galaxy::BuiltIn
{
	/// 执行
	void NotifyConditionAction::Execute()
	{
		std::unique_lock lock(*Mutex);
		*Flag = !Reverse;
		(*Condition).notify_all();
	}
}