#include "AbstractPort.hpp"

#include <utility>
#include "Tools/ProcessorAccess.hpp"

namespace Galaxy::Core
{
	/// 构造函数
	AbstractPort::AbstractPort(std::string  name, AbstractProcessor* host, bool optional) :
			MappingName(std::move(name)), Optional(optional)
	{
		Tools::ProcessorAccess::RegisterPort(host, this);
	}
}