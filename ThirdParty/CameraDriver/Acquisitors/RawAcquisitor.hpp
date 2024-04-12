#pragma once

#include "AbstractAcquisitor.hpp"

namespace RoboPioneers::Modules::CameraDriver::Acquisitors
{
	/**
	 * @brief 原始图像采集器
	 * @author Vincent
	 * @details
	 *  ~ 该采集器用于采集相机传递的原始图像，一般为BayerRG格式。
	 */
	class RawAcquisitor : public AbstractAcquisitor
	{
	protected:
		// 图片互斥量
		mutable std::shared_mutex PictureMutex {};
		/// 图片是否是最新的，即从未被获取过
		std::atomic_bool IsPictureLatest {false};
		/// 原始图片数据
		RawPicture Picture;

	public:
		/// 构造函数
		explicit RawAcquisitor(CameraDevice* device) :
			AbstractAcquisitor(device), Picture(nullptr, 0, 0)
		{}

		/**
		 * @brief 查询当前是否有新图片到达
		 * @retval true 当前图片未被获取过
		 * @retval false 当前图片已被获取过
		 */
		[[nodiscard]] bool HasNewPicture() const
		{
			return IsPictureLatest;
		}

		/// 接受到图片事件
		void ReceivePictureIncomeEvent(RawPicture data) override;

		/**
		 * @brief 获取原始图片
		 * @param wait_for_latest 是否阻塞调用线程直至新图片抵达
		 * @return 原始图片
		 * @throw std::runtime_error 当采集未开始或相机设备离线
		 */
		virtual RawPicture GetPicture(bool wait_for_latest) noexcept(false);
	};
}
