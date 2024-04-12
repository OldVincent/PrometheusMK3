#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/cudafilters.hpp>

namespace RoboPioneers::Prometheus::Processors
{
	/**
	 * @brief 开运算过滤器
	 * @author Vincent
	 * @details
	 *  ~ 该过滤器用于消除噪点。
	 */
	class OpenBlurProcessor AsProcessor
	{
	Requirement:
		Require(cv::cuda::GpuMat, FromGpuPicture);
		Require(cv::cuda::GpuMat, ToGpuPicture);
		RequireOptional(cv::cuda::Stream, GpuStream);

	public:
		/// 过滤器
		cv::Ptr<cv::cuda::Filter> Filter;
		/// 配置方法
		Configure(OpenBlurProcessor, int picture_type, int size,
			Name(FromGpuPicture), Name(ToGpuPicture), OptionalName(GpuStream))
		{
			ApplyName(FromGpuPicture);
			ApplyName(ToGpuPicture);

			auto kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(size,size));
			Filter = cv::cuda::createMorphologyFilter(cv::MORPH_OPEN, picture_type, kernel);
		}

		Process
		{
			Filter->apply(*FromGpuPicture, *ToGpuPicture, *GpuStream);
		};
	};
}