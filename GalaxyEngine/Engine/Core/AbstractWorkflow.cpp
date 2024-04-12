#include "AbstractWorkflow.hpp"
#include "AbstractProcessor.hpp"
#include "AbstractPort.hpp"
#include "AbstractChannel.hpp"
#include "AbstractExecutor.hpp"

#include "Tools/ProcessorAccess.hpp"
#include "Tools/PortAccess.hpp"

#include "../Runtime.hpp"
#include <stdexcept>

namespace Galaxy::Core
{
	/// 构造函数
	AbstractWorkflow::AbstractWorkflow() : InitializeTask(&Runtime::GetInstance()->CurrentExecutorPointer, this)
	{
		NextProcessor = Processors.begin();
	}

	/// 析构函数
	AbstractWorkflow::~AbstractWorkflow()
	{
		if (OnFinalize)
		{
			OnFinalize();
		}

		/// 遍历调用终止化方法
		for (auto reverse_index = Processors.rbegin(); reverse_index!= Processors.rend(); ++reverse_index)
		{
			Tools::ProcessorAccess::InvokeInitialize(std::get<0>(*reverse_index));
		}
	}

	/// 初始化方法
	void AbstractWorkflow::Initialize()
	{
		// 调用用户的初始化方法
		if (this->OnBegin)
		{
			OnBegin();
		}

		if (Initialized) return;

		// 遍历处理器
		for (const auto& [processor, executor] : Processors)
		{
			if (!processor)
			{
				throw std::runtime_error("[AbstractWorkflow::IterateExecute] A Processor Pointer is Null.");
			}

			// 遍历端口挂载
			for (const auto& port : Tools::ProcessorAccess::GetPorts(processor))
			{
				auto finder = this->Channels.find(port->MappingName);
				if (finder != this->Channels.end())
				{
					// 将端口挂载到通道
					Tools::PortAccess::AttachToChannel(port, finder->second);
				}
				else if (!port->IsOptional())
				{
					throw std::runtime_error(
							"[AbstractWorkflow::IterateExecute] A Channel Named " + port->MappingName + " is Missing.");
				}
			}

			Tools::ProcessorAccess::InvokeInitialize(processor);
		}
		// 将迭代器指向列表头部
		NextProcessor = Processors.begin();
		// 更新初始化状态
		Initialized = true;

		if (OnInitialize)
		{
			OnInitialize();
		}
	}

	/// 迭代执行方法
	std::optional<AbstractExecutor *> AbstractWorkflow::IterateExecute()
	{
		auto [current_processor, current_executor] = *NextProcessor;
		if (!current_executor)
		{
			throw std::runtime_error("[AbstractWorkflow::InvokeIterateExecute] Executor Pointer Reference is Null.");
		}
		if (!*current_executor)
		{
			throw std::runtime_error("[AbstractWorkflow::InvokeIterateExecute] Executor Pointer is Null.");
		}

		bool stop_flag = Tools::ProcessorAccess::IsStopFlagOn(current_processor);
		bool pause_flag = Tools::ProcessorAccess::IsPauseFlagOn(current_processor);
		if (stop_flag || pause_flag)
		{
			Tools::ProcessorAccess::ResetFlags(current_processor);
		}

		Tools::ProcessorAccess::InvokeExecute(current_processor);

		++NextProcessor;

		if (NextProcessor != Processors.end())
		{
			if (stop_flag)
			{
				if (OnEnd)
				{
					OnEnd();
				}
			}
			if (stop_flag || pause_flag)
			{
				return std::nullopt;
			}
		}
		// 若迭代已达末尾
		else
		{
			// 将迭代器指向开头
			NextProcessor = Processors.begin();

			// 若设置了结束事件，则执行
			if (OnEnd)
			{
				OnEnd();
			}

			// 判断是否允许终止
			if (Loop && !pause_flag)
			{
				// 若设置了终止条件，则核验终止条件
				if (LoopStopCondition)
				{
					if (LoopStopCondition())
					{
						// 满足终止条件，返回空
						return std::nullopt;
					}
				}

				// 触发开始事件
				if (OnBegin)
				{
					OnBegin();
				}
			}
			else
			{
				// 若未开启循环，则返回空
				return std::nullopt;
			}
		}
		return {*std::get<1>(*NextProcessor)};
	}

	/// 流传出操作符
	AbstractWorkflow &AbstractWorkflow::operator>>(AbstractExecutor *executor)
	{
		executor->Submit(this);

		return *this;
	}

	/// 流传出操作符
	AbstractWorkflow &AbstractWorkflow::operator>>(AbstractExecutor &executor)
	{
		executor.Submit(this);

		return *this;
	}
}