#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <string>

namespace RoboPioneers::Prometheus::Processors::DebugPackage
{
	/**
	 * @brief 图像显示流处理器
	 * @author Vincent
	 * @details
	 *  ~ 该流处理器用于显示图像。
	 *  ~ 该流处理器将显示cv::Mat类型的DisplayPicture通道。
	 */
	class MatView AsProcessor
	{
	Requirement:
		/// 输出图像
		Require(cv::Mat, Picture);

	public:
		/// 窗口标题
		std::string Title;

		/// 构造函数
		Configure(MatView, const std::string& target_name), Title(target_name)
		{
			Picture.MappingName = target_name;
		}

		/// 执行方法
		Process
		{
			cv::imshow(Title, Picture.Acquire());
		}
	};
}
