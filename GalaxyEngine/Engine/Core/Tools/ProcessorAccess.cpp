#include "ProcessorAccess.hpp"
#include "../AbstractProcessor.hpp"

namespace Galaxy::Core::Tools
{
	/// 调用处理器执行方法
	void ProcessorAccess::InvokeExecute(AbstractProcessor *processor)
	{
		processor->Execute();
	}

	/// 注册端口
	void ProcessorAccess::RegisterPort(AbstractProcessor *processor, AbstractPort *port)
	{
		processor->Ports.emplace(port);
	}

	/// 获取端口
	auto ProcessorAccess::GetPorts(AbstractProcessor* processor)
		-> tbb::concurrent_unordered_set<AbstractPort *> &
	{
		return processor->Ports;
	}

	/// 调用初始化事件
	void ProcessorAccess::InvokeInitialize(AbstractProcessor *processor)
	{
		processor->OnInitialize();
	}

	/// 调用终止化事件
	void ProcessorAccess::InvokeFinalize(AbstractProcessor *processor)
	{
		processor->OnFinalize();
	}

	/// 判断暂停旗标是否已经开启
	bool ProcessorAccess::IsPauseFlagOn(AbstractProcessor *processor)
	{
		return processor->PauseWorkflowFlag;
	}

	/// 判断停止旗标是否已经开启
	bool ProcessorAccess::IsStopFlagOn(AbstractProcessor *processor)
	{
		return processor->StopWorkflowFlag;
	}

	/// 重设暂停旗标
	void ProcessorAccess::ResetFlags(AbstractProcessor *processor)
	{
		processor->StopWorkflowFlag = false;
		processor->PauseWorkflowFlag = false;
	}
}