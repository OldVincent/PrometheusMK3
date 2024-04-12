#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>

namespace RoboPioneers::Prometheus::Processors
{
	/**
	 * @brief GPU图像上传器
	 * @author Vincent
	 * @details
	 *  ~ 会将cv::Mat类型的Picture通道的图片上传到显存中，并且传入到cv::cuda::GpuMat类型的GpuPicture通道中。
	 *  ~ 第一个配置选项为被上传的图像所在通道，第二个配置选项为目标显存图像所在通道，第三个配置选项为GPU流所在通道。
	 */
	class GpuPictureUploader AsProcessor
	{
	Requirement:
		/// 被上传的图片
		Require(cv::Mat, Picture);
		/// 目标显存图片
		Require(cv::cuda::GpuMat, GpuPicture);
		/// 显存流
		RequireOptional(cv::cuda::Stream, GpuStream);

	public:
		/// 配置
		Configure(GpuPictureUploader, Name(Picture), Name(GpuPicture), OptionalName(GpuStream))
		{
			ApplyName(Picture);
			ApplyName(GpuPicture);
			ApplyName(GpuStream);
		}

		/// 裁剪图片操作
		Process
		{
			if (GpuStream.IsMounted())
			{
				GpuPicture.Acquire().upload(Picture.Acquire(), GpuStream.Acquire());
			}
			else
			{
				GpuPicture.Acquire().upload(Picture.Acquire());
			}

		}
	};
}
