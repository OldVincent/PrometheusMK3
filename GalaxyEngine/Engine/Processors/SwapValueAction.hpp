#pragma once

#include "../../Framework/Port.hpp"
#include "../../Framework/Processor.hpp"
#include "../../Framework/Channel.hpp"

#include "../../Engine/Core/Tools/WorkflowAccess.hpp"

#include <utility>

namespace Galaxy::BuiltIn
{
	/**
	 * @brief 切换值的处理器
	 * @tparam ValueType 值类型
	 * @author Vincent
	 * @details
	 *  ~ 该处理器用于交换两个通道的值。
	 */
	template<typename ValueType>
	class SwapValueAction : public Core::AbstractProcessor
	{
	private:
		/// 通道1的名称
		std::string ChannelName1;
		/// 通道2的名称
		std::string ChannelName2;

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
		SwapValueAction(ExecutorType** target_executor, WorkflowType* host, std::string name1, std::string name2) :
		Core::AbstractProcessor((Core::AbstractExecutor**)(target_executor), (Core::AbstractWorkflow*)(host)),
			ChannelName1(std::move(name1)), ChannelName2(std::move(name2))
		{}

	protected:
		/// 执行操作
		void Execute() override
		{
			auto* channel1 = static_cast<Channel<ValueType>*>(
					Core::Tools::WorkflowAccess::GetChannel(GetWorkflow(), ChannelName1));
			auto* channel2 = static_cast<Channel<ValueType>*>(
					Core::Tools::WorkflowAccess::GetChannel(GetWorkflow(), ChannelName2));
			auto channel1_value = channel1->Acquire();
			channel1->Acquire() = channel2->Acquire();
			channel2->Acquire() = channel1_value;
		}
	};
}
