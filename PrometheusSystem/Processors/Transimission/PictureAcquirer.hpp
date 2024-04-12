#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <CameraDriver/CameraDriver.hpp>

namespace RoboPioneers::Prometheus::Processors
{
	/**
	 * @brief 图像获取流处理器
	 * @author Vincent
	 * @details
	 *  ~ 该流处理器用于获取图像。
	 *  ~ 该流处理器会将图片存储到cv::Mat类型的Picture通道。
	 *  ~ 注意，所有的图像获取流处理器共享同一个相机对象和采集器。
	 */
	class PictureAcquirer AsProcessor
	{
	Requirement:
		/// 输出图像
		Require(cv::Mat, Picture);

	protected:
		/// 初始化方法，将开启相机设备和采集器
		void OnInitialize() override;
		/// 终止化方法，将关闭相机和采集器
		void OnFinalize() override;

		int CameraExposureMicroSeconds = 1000;
		int CameraGain = 16;

	public:
		/// 相机索引
		unsigned int CameraIndex;
		/// 是否等待最新图片
		bool WaitForLatest;
		/// 相机离线等待上限周期
		unsigned int WaitingSeconds;

		/// 构造函数
		Configure(PictureAcquirer,
			unsigned int camera_index = 0,
			bool waiting_for_latest = true,
			unsigned int waiting_seconds = 10),
			CameraIndex(camera_index), WaitForLatest(waiting_for_latest), WaitingSeconds(waiting_seconds)
		{}

	public:
		/// 执行方法
		void Execute() override;
	};
}