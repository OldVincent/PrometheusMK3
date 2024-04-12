#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <vector>

namespace RoboPioneers::Prometheus::Processors
{
	/**
	 * @brief 轮廓检测器
	 * @author Vincent
	 * @details
	 *  ~ 该流处理器用于从二值图中检测轮廓。
	 */
	class ContoursDetector AsProcessor
	{
	Requirement:
		/// 用于检测轮廓的二值图
		Require(cv::Mat, BinaryPicture);
		/// 轮廓列表
		Require(std::vector<std::vector<cv::Point>>, Contours);

	public:
		/**
		 * @brief 构造函数
		 * @param enhance_mode 控制是否开启轮廓增强模式，详见EnhanceMode成员
		 */
		Configure(ContoursDetector)
		{}

		/// 执行操作
		void Execute() override;
	};
}
