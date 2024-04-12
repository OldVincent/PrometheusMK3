#pragma once

#include "GalaxyEngine/Engine/Core/AbstractExecutor.hpp"

namespace Galaxy
{
	/**
	 * @brief 串行执行器
	 * @author Vincent
	 * @details
	 *  ~ 该执行器将串行地执行任务。
	 */
	class SerialExecutor : public Core::AbstractExecutor
	{
	public:
		using AbstractExecutor::AbstractExecutor;

	protected:
		/// 更新事件，将从队列中取出一个工作流并执行
		void OnUpdateWorkingThread() override;
	};
}