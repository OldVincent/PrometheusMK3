#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>

namespace RoboPioneers::Prometheus::Processors
{
	/**
	 * @brief 等待GPU流完成
	 * @author Vincent
	 * @details
	 *  ~ 该流处理器用于在CPU和GPU间同步。
	 */
	class WaitGPUStream AsProcessor
	{
	Requirement:
		Require(cv::cuda::Stream, GpuStream);

	public:
		void Execute() override
		{
			(*GpuStream).waitForCompletion();
		}
	};
}