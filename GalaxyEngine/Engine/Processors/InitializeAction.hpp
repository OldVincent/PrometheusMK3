#pragma once

#include "../Core/AbstractProcessor.hpp"

namespace Galaxy::BuiltIn
{
	/**
	 * @brief 工作流初始化处理器
	 * @author Vincent
	 * @details
	 *  ~ 该流处理器将会调用工作流的初始化方法。
	 */
	class InitializeAction : public Core::AbstractProcessor
	{
	public:
		using Core::AbstractProcessor::AbstractProcessor;

	protected:
		/// 执行方法
		void Execute() override;
	};
}