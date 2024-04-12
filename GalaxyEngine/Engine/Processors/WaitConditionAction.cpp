#include "WaitConditionAction.hpp"

namespace Galaxy::BuiltIn
{
	/// 执行方法
	void WaitConditionAction::Execute()
	{
		std::unique_lock lock((*Mutex));
		(*Condition).wait(lock, [flag = &this->Flag, reverse = this->Reverse]{
			return flag->Acquire() != reverse;});
		(*Flag) = Reverse;
		lock.unlock();
	}
}