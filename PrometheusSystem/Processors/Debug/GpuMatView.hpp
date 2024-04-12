#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <string>

namespace RoboPioneers::Prometheus::Processors::DebugPackage
{
	/**
	 * @brief 显存图像显示流处理器
	 * @author Vincent
	 * @details
	 *  ~ 该流处理器用于显示显存中的图像。
	 *  ~ 该流处理器将显示cv::cuda::GpuMat类型的DisplayGpuPicture通道。
	 */
	class GpuMatView AsProcessor
	{
	Requirement:
		/// 输出图像
		Require(cv::cuda::GpuMat, GpuPicture);
		/// 输出图像的流
		RequireOptional(cv::cuda::Stream, GpuStream);

	public:
		/// 窗口标题
		std::string Title;

		/// 构造函数
		Configure(GpuMatView, std::string target_name), Title(target_name)
		{
			GpuPicture.MappingName = target_name;
		}

		/// 执行方法
		Process
		{
			cv::Mat picture(GpuPicture.Acquire().size(), GpuPicture.Acquire().type(),
				   cv::Scalar(0, 0, 0));

			// 进行流同步，等待图片传输完成
			if (GpuStream.IsMounted())
			{
				GpuPicture.Acquire().download(picture, GpuStream.Acquire());
				GpuStream.Acquire().waitForCompletion();
			}
			{
				cv::cuda::Stream::Null().waitForCompletion();
				// 注意，若图片正在默认流以外的流中处理，则此处会发生内存错误访问
				GpuPicture.Acquire().download(picture);
			}
			cv::imshow(Title, picture);
		}
	};
}
