#include "LambdaAction.hpp"

namespace Galaxy::BuiltIn
{
	/// 执行方法
	void LambdaAction::Execute()
	{
		if (Operation)
		{
			Operation();
		}
	}
}