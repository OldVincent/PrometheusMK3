#pragma once

#include <shared_mutex>
#include <atomic>
#include <opencv4/opencv2/opencv.hpp>

#include "AbstractAcquisitor.hpp"

namespace RoboPioneers::Modules::CameraDriver::Acquisitors
{
	/**
	 * @brief Bayer格式的cv::Mat格式采集器
	 * @author Vincent
	 * @details
	 *  ~ 该类用于从相机采集cv::Mat格式的对象，颜色格式未发生转换，一般为BayerRG格式，具体需要查阅相机说明书。
	 */
	class BayerMatAcquisitor : public AbstractAcquisitor
	{
	protected:
		/// 图片互斥量
		mutable std::shared_mutex PictureMutex {};
		/// 图片是否是最新的，即从未被获取过
		std::atomic_bool IsPictureLatest {false};
		/// 图片对象，格式CV_8UC1
		cv::Mat Picture {};

	protected:
		/**
		 * @brief 将原始图像转换为Mat矩阵
		 * @param data 原始图像
		 * @return 转换后的cv::Mat
		 */
		virtual cv::Mat ConvertRawDataToPicture(const RawPicture& data);

	public:
		/**
		 * @brief 构造函数
		 * @param device 执行采集操作的相机设备对象
		 */
		explicit BayerMatAcquisitor(CameraDevice* device) : AbstractAcquisitor(device)
		{}

		//==============================
		// 采集器基本控制方法
		//==============================

		/**
		 * @brief 查询当前是否有新图片
		 * @return 若当前持有的图片未被获取过，则返回true，否则返回false。
		 */
		[[nodiscard]] bool HasNewPicture() const noexcept
		{
			return IsPictureLatest.load();
		}

		/**
		 * @brief 获取图片
		 * @param wait_for_latest 是否阻塞当前线程直到采集到新的图片，若为false，则函数将直接返回，无论图片是否是最新的
		 * @return 采集到的图片，格式为CV_8UC1
		 * @throw std::logic_error 当设备未开始采集时调用该方法将抛出该异常
		 * @throw std::runtime_error 当设备开始采集但却异常离线时调用方法将抛出该异常
		 */
		virtual cv::Mat GetPicture(bool wait_for_latest) noexcept(false);

		//==============================
		// 事件处理方法
		//==============================

		/**
		 * @brief 接收到图片事件
		 * @param data 原始图片数据
		 * @details
		 *  ~ 当采集到图片并处理完毕时，该方法将被采集线程调用。
		 */
		void ReceivePictureIncomeEvent(AbstractAcquisitor::RawPicture data) override;
	};
}