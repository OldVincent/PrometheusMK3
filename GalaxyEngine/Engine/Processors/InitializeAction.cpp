#include "InitializeAction.hpp"
#include "../Core/AbstractWorkflow.hpp"
#include "../Core/Tools/WorkflowAccess.hpp"

namespace Galaxy
{
	/// 执行方法
	void BuiltIn::InitializeAction::Execute()
	{
		Core::Tools::WorkflowAccess::Initialize(GetWorkflow());
	}
}