#include "SerialCommand.hpp"
#include "../../Modules/CRCModule.hpp"

namespace RoboPioneers::Prometheus::Processors
{
	void SerialCommand::Execute()
	{
		std::vector<unsigned char> data;
		data.resize(12);

		data[0] = 0xFF;
		*reinterpret_cast<char*>(&data[1]) = *Command;
		*reinterpret_cast<int*>(&data[2]) = *X;
		*reinterpret_cast<int*>(&data[6]) = *Y;
		*reinterpret_cast<char*>(&data[10]) = *Number;
		data[11] = Modules::CRCModule::GetCRC8CheckSum(data.data(), 11);

		Port.Write(data);
	}

	void SerialCommand::OnInitialize()
	{
		Port.Open(PortName);
		Port.SetBaudRate(115200);
		Port.SetParityType(RoboPioneers::Modules::SerialPortDriver::SerialPort::parity::none);
		Port.SetStopBitsType(RoboPioneers::Modules::SerialPortDriver::SerialPort::stop_bits::one);
		Port.SetCharacterSize(8);
	}
}
