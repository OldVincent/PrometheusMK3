#include "AbstractProcessor.hpp"
#include "AbstractWorkflow.hpp"
#include "Tools/WorkflowAccess.hpp"

namespace Galaxy::Core
{
	/// 构造函数，用于自动注册
	AbstractProcessor::AbstractProcessor(AbstractExecutor** target_executor, AbstractWorkflow *host) :
			TargetExecutor(target_executor), HostWorkflow(host)
	{
		Tools::WorkflowAccess::RegisterProcessor(host, this, target_executor);
	}
}