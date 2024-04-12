#pragma once

#include "../../Framework/Port.hpp"
#include "../../Framework/Processor.hpp"
#include "../../Framework/Channel.hpp"

#include "../../Engine/Core/Tools/WorkflowAccess.hpp"

#include <utility>

namespace Galaxy::BuiltIn
{
	/**
	 * @brief 传递值的处理器
	 * @tparam ValueType 值类型
	 * @author Vincent
	 * @details
	 *  ~ 该处理器用于将一个通道的值传递进另一个通道。
	 */
	template<typename ValueType>
	class PassValueAction : public Core::AbstractProcessor
	{
	private:
		/// 通道1的名称
		std::string ChannelNameFrom;
		/// 通道2的名称
		std::string ChannelNameTo;

	public:
		/**
		 * @brief 构造函数
		 * @param target_executor 目标执行器
		 * @param host 宿主
		 * @param name1 需要交换值的一个通道的名称
		 * @param name2 需要交换值的另一个通道的名称
		 */
		template<typename ExecutorType, typename WorkflowType,
				typename = typename std::enable_if<std::is_base_of_v<Core::AbstractExecutor, ExecutorType>>,
				typename = typename std::enable_if<std::is_base_of_v<Core::AbstractWorkflow, WorkflowType>>>
		PassValueAction(ExecutorType** target_executor, WorkflowType* host,
				  std::string from_name, std::string to_name) :
		Core::AbstractProcessor((ExecutorType**)(target_executor), (WorkflowType*)(host)),
				ChannelNameFrom(std::move(from_name)), ChannelNameTo(std::move(to_name))
		{}

	protected:
		/// 执行操作
		void Execute() override
		{
			auto* channel_from = static_cast<Channel<ValueType>*>(
					Core::Tools::WorkflowAccess::GetChannel(GetWorkflow(), ChannelNameFrom));
			auto* channel_to = static_cast<Channel<ValueType>*>(
					Core::Tools::WorkflowAccess::GetChannel(GetWorkflow(), ChannelNameTo));

			channel_to->Acquire() = channel_from->Acquire();
		}
	};
}
