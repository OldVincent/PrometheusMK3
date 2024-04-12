#pragma once

#include "../Core/AbstractProcessor.hpp"
#include "../Core/AbstractExecutor.hpp"
#include "../Core/AbstractWorkflow.hpp"
#include <type_traits>
#include <utility>

namespace Galaxy
{
	template<typename ProcessorClass,
			typename = typename std::enable_if<std::is_base_of_v<Core::AbstractProcessor, ProcessorClass>>>
	class DecoratorIf : public Core::AbstractProcessor
	{
	private:
		/// 被装饰的流处理器
		ProcessorClass Processor;

	public:
		/// 执行条件
		std::function<bool()> ExecuteCondition;

		/// 构造函数，将构造内部的流处理器
		DecoratorIf(Core::AbstractExecutor** target_executor,
					Core::AbstractWorkflow* host, decltype(ExecuteCondition) condition) :
			Processor(target_executor, host), ExecuteCondition(std::move(condition))
		{}

	protected:
		/// 执行
		void Execute() override
		{
			if (ExecuteCondition && ExecuteCondition())
			{
				Processor.Execute();
			}
		}
	};
}