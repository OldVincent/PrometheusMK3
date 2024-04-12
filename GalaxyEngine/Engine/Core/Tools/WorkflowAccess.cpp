#include "WorkflowAccess.hpp"

#include "../AbstractExecutor.hpp"
#include "../AbstractWorkflow.hpp"
#include "../AbstractChannel.hpp"

namespace Galaxy::Core::Tools
{
	/// 注册流处理器
	void WorkflowAccess::RegisterProcessor(AbstractWorkflow *workflow, AbstractProcessor *processor,
	                                       AbstractExecutor **executor)
	{
		workflow->Processors.emplace_back(processor, executor);
	}

	/// 注册通道
	void WorkflowAccess::RegisterChannel(AbstractWorkflow *workflow, AbstractChannel *channel, const std::string& name)
	{
		workflow->Channels.emplace(name, channel);
	}

	/// 迭代执行
	auto WorkflowAccess::IterateExecute(AbstractWorkflow *workflow) -> std::optional<AbstractExecutor *>
	{
		return workflow->IterateExecute();
	}

	/// 初始化
	void WorkflowAccess::Initialize(AbstractWorkflow *workflow)
	{
		workflow->Initialize();
	}

	/// 获取通道
	AbstractChannel *WorkflowAccess::GetChannel(AbstractWorkflow *workflow, const std::string& name)
	{
		auto finder = workflow->Channels.find(name);
		if (finder != workflow->Channels.end())
		{
			return finder->second;
		}
		return nullptr;
	}

	/// 注册通道
	void WorkflowAccess::RegisterChannel(AbstractWorkflow *workflow, AbstractChannel *channel,
	                                     const std::initializer_list<const char *> &names)
	{
		for (const auto& name : names)
		{
			workflow->Channels.emplace(name, channel);
		}
	}

	/// 获取当前执行器
	AbstractExecutor* WorkflowAccess::GetCurrentExecutor(AbstractWorkflow *workflow)
	{
		if (workflow->NextProcessor != workflow->Processors.end())
		{
			auto& [processor, executor] = *(workflow->NextProcessor);
			if (executor)
			{
				return *executor;
			}
		}
		return nullptr;
	}
}