#pragma once

#include "../../Framework/Processor.hpp"
#include "../../Framework/Port.hpp"
#include "../MacroKeywords.hpp"
#include <mutex>
#include <condition_variable>

namespace Galaxy::BuiltIn
{
	/**
	 * @brief 等待条件变量处理器
	 * @author Vincent
	 * @details
	 *  ~ 该流处理器用于等待条件变量。
	 *  ~ 需要三个通道，条件变量通道、互斥量通道、旗标通道。
	 */
	class WaitConditionAction AsProcessor
	{
	Requirement:
		Require(std::condition_variable, Condition);
		Require(std::mutex, Mutex);
		Require(bool, Flag);

	private:
		/**
		 * @brief 是否逆转
		 * @details
		 *  ~ 该值默认为false，即不逆转，等待条件值为true，苏醒后将其设置为false。
		 *  ~ 设置为true，即开启逆转，等待条件值为false，苏醒后将其设置为true。
		 */
		bool Reverse;

	public:
		Configure(WaitConditionAction, Name(Condition), Name(Mutex), Name(Flag), bool reverse = false),
			Reverse(reverse)
		{
			ApplyName(Condition);
			ApplyName(Mutex);
			ApplyName(Flag)
		}

	protected:
		/// 执行方法
		void Execute() override;
	};
}