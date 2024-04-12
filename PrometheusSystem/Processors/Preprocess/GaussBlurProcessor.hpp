#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/cudafilters.hpp>

namespace RoboPioneers::Prometheus::Processors
{
	/**
	 * @brief 高斯模糊
	 * @author Vincent
	 * @details
	 *  ~ 该流处理会使用GpuPicture通道和可选的GpuStream通道。
	 */
	class GaussBlurProcessor AsProcessor
	{
	Requirement:
		Require(cv::cuda::GpuMat, FromGpuPicture);
		Require(cv::cuda::GpuMat, ToGpuPicture);
		RequireOptional(cv::cuda::Stream, GpuStream);

	public:
		/// 过滤器
		cv::Ptr<cv::cuda::Filter> Filter;

		Configure(GaussBlurProcessor, int picture_type,
			Name(FromGpuPicture), Name(ToGpuPicture), OptionalName(GpuStream))
		{
			ApplyName(FromGpuPicture);
			ApplyName(ToGpuPicture);
			Filter = cv::cuda::createGaussianFilter(picture_type, picture_type,cv::Size(3,3), 0.8);
		}

		Process
		{
			auto& from = *FromGpuPicture;
			auto& to = *ToGpuPicture;

			Filter->apply(*FromGpuPicture, *ToGpuPicture, *GpuStream);
		}
	};
}