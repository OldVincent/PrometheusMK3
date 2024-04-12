#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <utility>
#include <string>
#include <SerialPortDriver/SerialPortDriver.hpp>

namespace RoboPioneers::Prometheus::Processors
{
	/**
	 * @brief 图像获取流处理器
	 * @author Vincent
	 * @details
	 *  ~ 该流处理器用于获取图像。
	 *  ~ 该流处理器会将图片存储到cv::Mat类型的Picture通道。
	 *  ~ 注意，所有的图像获取流处理器共享同一个相机对象和采集器。
	 */
	class SerialCommand AsProcessor
	{
	Requirement:
		/// 指令通道
		Require(char, Command);
		/// 横坐标
		Require(int, X);
		/// 纵坐标
		Require(int, Y);
		/// 数字识别
		Require(char, Number);

	public:
		Modules::SerialPortDriver::SerialPort Port;

		std::string PortName;
	public:
		/// 配置
		Configure(SerialCommand, std::string serial_port_name), PortName(std::move(serial_port_name))
		{}

	protected:
		void OnInitialize() override;

	public:
		/// 执行方法
		void Execute() override;
	};
}

