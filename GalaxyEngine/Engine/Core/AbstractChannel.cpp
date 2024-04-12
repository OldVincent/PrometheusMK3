#include "AbstractChannel.hpp"
#include "AbstractWorkflow.hpp"
#include "Tools/WorkflowAccess.hpp"

namespace Galaxy::Core
{
	/// 构造函数，主要用于注册自身
	AbstractChannel::AbstractChannel(AbstractWorkflow *host, const std::initializer_list<const char*>& names)
	{
		Tools::WorkflowAccess::RegisterChannel(host, this, names);
	}

	/// 构造函数，主要用于注册自身
	AbstractChannel::AbstractChannel(AbstractWorkflow *host, const char* name)
	{
		// 注册自身
		Tools::WorkflowAccess::RegisterChannel(host, this, name);
	}
}