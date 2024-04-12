#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>

namespace RoboPioneers::Prometheus::Processors
{
	/**
	 * @brief GPU图像下载器
	 * @author Vincent
	 * @details
	 *  ~ 该流处理器用于将图片从显存中下载到内存中。
	 */
	class GpuPictureDownloader AsProcessor
	{
	Requirement:
		/// 目标中的图片
		Require(cv::cuda::GpuMat, GpuPicture);
		/// 下载目标的图片
		Require(cv::Mat, Picture);
		/// 显存流
		RequireOptional(cv::cuda::Stream, GpuStream);

	public:
		/// 配置
		Configure(GpuPictureDownloader, Name(GpuPicture), Name(Picture), OptionalName(GpuStream))
		{
			ApplyName(GpuPicture);
			ApplyName(Picture);
			ApplyName(GpuStream);
		}

		/// 裁剪图片操作
		Process
		{
			if (GpuStream.IsMounted())
			{
				(*GpuPicture).download(*Picture, *GpuStream);
			}
			else
			{
				(*GpuPicture).download(*Picture);
			}

		}
	};
}
