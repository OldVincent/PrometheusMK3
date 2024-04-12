#pragma once

#include "BayerMatAcquisitor.hpp"

namespace RoboPioneers::Modules::CameraDriver::Acquisitors
{
	/**
	 * @brief cv::Mat格式采集器
	 * @author Vincent
	 * @details
	 *  ~ 该类用于从相机采集cv::Mat格式的对象。
	 */
	class MatAcquisitor : public BayerMatAcquisitor
	{
	protected:
		/**
		 * @brief 将原始图像转换为Mat矩阵
		 * @param data 原始图像
		 * @return 转换后的cv::Mat，格式为CV_8UC3
		 */
		cv::Mat ConvertRawDataToPicture(const RawPicture& data) override;

	public:
		/**
		 * @brief 构造函数
		 * @param device 执行采集操作的相机设备对象
		 */
		explicit MatAcquisitor(CameraDevice* device) : BayerMatAcquisitor(device)
		{}
	};
}
