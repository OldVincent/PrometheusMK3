#pragma once

#include <string>
#include <tbb/tbb.h>

namespace Galaxy::Core
{
	class AbstractProcessor;
	class AbstractPort;

	namespace Tools
	{
		/**
	 	 * @brief 流处理器访问器
	 	 * @author Vincent
	 	 * @details
	 	 *  ~ 该工具类用于在不增加用户心智负担的情况下减少友元访问声明。
	 	 */
		class ProcessorAccess
		{
		public:
			/// 调用执行方法
			static void InvokeExecute(AbstractProcessor* processor);
			/// 调用初始化方法
			static void InvokeInitialize(AbstractProcessor* processor);
			/// 调用终止化方法
			static void InvokeFinalize(AbstractProcessor* processor);
			/// 注册端口
			static void RegisterPort(AbstractProcessor* processor, AbstractPort* port);
			/// 获取端口
			static auto GetPorts(AbstractProcessor* processor)
				-> tbb::concurrent_unordered_set<AbstractPort*>&;

			/// 判断中断旗标是否开启
			static bool IsPauseFlagOn(AbstractProcessor* processor);
			/// 判断停止旗标是否开始
			static bool IsStopFlagOn(AbstractProcessor* processor);
			//// 重设所有的旗标，当旗标起效后该方法将被调用
			static void ResetFlags(AbstractProcessor* processor);
		};
	}
}