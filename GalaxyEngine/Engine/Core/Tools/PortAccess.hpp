#pragma once

#include <string>

namespace Galaxy::Core
{
	class AbstractChannel;
	class AbstractPort;

	namespace Tools
	{
		/**
	 	 * @brief 流处理器访问器
	 	 * @author Vincent
	 	 * @details
	 	 *  ~ 该工具类用于在不增加用户心智负担的情况下减少友元访问声明。
	 	 */
		class PortAccess
		{
		public:
			/// 将端口附加到通道
			static void AttachToChannel(AbstractPort* port, AbstractChannel* channel);
			/// 获取名称
			static auto GetName(AbstractPort* port) -> const std::string&;
		};
	}
}