#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>

namespace RoboPioneers::Prometheus::Processors::DebugPackage
{
	/**
	 * @brief 按键等待器
	 * @author Vincent
	 * @details
	 *  ~ 使用OpenCV的方法等待按键。
	 */
	class StopKeyWaiter AsProcessor
	{
	public:
		/// 停止按键，当按下该按键时工作流的循环将终止
		int StopKey;

		/// 构造函数
		Configure(StopKeyWaiter, int stop_key = 27), StopKey(stop_key)
		{}

		/// 执行操作
		void Execute() override;
	};
}