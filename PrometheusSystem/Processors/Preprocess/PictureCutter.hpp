#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <list>

namespace RoboPioneers::Prometheus::Processors
{
	/**
	 * @brief 图像裁剪器
	 * @author Vincent
	 * @details
	 *  ~ 该流处理器用于裁剪图像。
	 *  ~ 该流处理器会使用cv::Rect类型的CuttingArea通道的区域参数，将cv::Mat类型的CuttingPicture通道裁剪。
	 */
	class PictureCutter AsProcessor
	{
	Requirement:
		/// 裁剪目标区域
		Require(cv::Rect, CuttingArea);
		/// 被裁剪的目标
		Require(cv::Mat, CuttingPicture);
		/// 坐标偏移
		Require(cv::Point, PositionOffset);

	public:
		/// 裁剪图片操作
		void Execute() override;
	};
}
