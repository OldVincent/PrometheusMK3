#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/cudaimgproc.hpp>

namespace RoboPioneers::Prometheus::Processors
{
	/**
	 * @brief 颜色转换器流处理器
	 * @author Vincent
	 * @details
	 *  ~ 该流将使用cv::cuda::GpuMat类型的GpuPicture通道。
	 */
	class BayerBGToHSVConverter AsProcessor
	{
	Requirement:
		/// 源图像
		Require(cv::cuda::GpuMat, FromGpuPicture);
		/// 目标图像
		Require(cv::cuda::GpuMat, ToGpuPicture);
		/// 显卡流
		RequireOptional(cv::cuda::Stream, GpuStream);

	public:
		/// 指定源通道和目标通道的名称
		Configure(BayerBGToHSVConverter, Name(FromGpuPicture), Name(ToGpuPicture), OptionalName(GpuStream))
		{
			ApplyName(FromGpuPicture);
			ApplyName(ToGpuPicture);
			ApplyName(GpuStream);
		}

		/// 执行
		Process
		{
			if (GpuStream.IsMounted())
			{
				/// BayerBG转BGR
				cv::cuda::cvtColor(FromGpuPicture.Acquire(), ToGpuPicture.Acquire(), cv::COLOR_BayerBG2BGR,
				                   0, GpuStream.Acquire());
				/// BGR转HSV
				cv::cuda::cvtColor(FromGpuPicture.Acquire(), ToGpuPicture.Acquire(), cv::COLOR_BGR2HSV,
				                   0, GpuStream.Acquire());
			}
			else
			{
				/// BayerBG转BGR
				cv::cuda::cvtColor(FromGpuPicture.Acquire(), ToGpuPicture.Acquire(), cv::COLOR_BayerBG2BGR);
				/// BGR转HSV
				cv::cuda::cvtColor(FromGpuPicture.Acquire(), ToGpuPicture.Acquire(), cv::COLOR_BGR2HSV);
			}
		}
	};
}