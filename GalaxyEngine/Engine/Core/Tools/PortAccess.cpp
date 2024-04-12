#include "PortAccess.hpp"
#include "../AbstractPort.hpp"

namespace Galaxy::Core::Tools
{
	/// 将端口附加到通道
	void PortAccess::AttachToChannel(AbstractPort *port, AbstractChannel *channel)
	{
		port->MountedChannel = channel;
	}

	/// 获取名称
	auto PortAccess::GetName(AbstractPort *port) -> const std::string &
	{
		return port->MappingName;
	}
}
