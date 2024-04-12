#pragma once

#include "../Core/AbstractProcessor.hpp"

namespace Galaxy::BuiltIn
{
	/**
	 * @brief 空流处理器
	 * @author Vincent
	 * @details
	 *  ~ 该流处理器将会直接将工作流转移到目标执行器。
	 */
	class EmptyProcessor : public Core::AbstractProcessor
	{
	public:
		using Core::AbstractProcessor::AbstractProcessor;

	protected:
		/// 执行方法
		void Execute() override {}
	};
}