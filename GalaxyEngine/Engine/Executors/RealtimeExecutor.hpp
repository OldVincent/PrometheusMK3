#pragma once

#include "../Core/AbstractExecutor.hpp"

namespace Galaxy
{
	/**
	 * @brief 实时执行器
	 * @author Vincent
	 * @details
	 *  ~ 实时执行器在拿到工作流后会立即执行。
	 *  ~ 实时执行器只适合执行简单的操作。
	 */
	class RealtimeExecutor : public Core::AbstractExecutor
	{
	protected:
		/// 工作线程更新方法，将直接返回
		void OnUpdateWorkingThread() override;

	public:
		/**
		 * @brief 提交方法
		 * @throw std::runtime_error 当工作流指针为空
		 * @param workflow 工作流指针
		 * @details
		 *  ~ 工作流会在调用线程中被立即执行。
		 */
		void Submit(Core::AbstractWorkflow *workflow) override;
	};
}