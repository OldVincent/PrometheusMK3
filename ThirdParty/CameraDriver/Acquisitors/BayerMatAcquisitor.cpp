#include "BayerMatAcquisitor.hpp"

#include <DxImageProc.h>
#include <stdexcept>
#include <thread>

namespace RoboPioneers::Modules::CameraDriver::Acquisitors
{
	/// 接受采集到图片的事件
	void BayerMatAcquisitor::ReceivePictureIncomeEvent(AbstractAcquisitor::RawPicture data)
	{
		if (!Working.load())
		{
			Working = true;
		}

		// 更新最新图片状态
		if (!IsPictureLatest.load())
		{
			IsPictureLatest = true;
		}

		auto&& picture = ConvertRawDataToPicture(data);

		std::unique_lock lock(PictureMutex);

		Picture = std::move(picture);
	}

	/// 获取新采集的图片
	cv::Mat BayerMatAcquisitor::GetPicture(bool wait_for_latest)
	{
		if (!IsWorking())
		{
			throw std::runtime_error("MatAcquisitor::GetPicture Device Is Offline.");
		}

		// 如果要求等待，则会不断地核验图片是否为最新
		while (wait_for_latest && !IsPictureLatest.load())
		{
			// 等待时交出当前的时间片，处理器会将分配给该线程资源暂时交给别的县城
			std::this_thread::yield();
		}

		std::shared_lock lock(PictureMutex);
		// 若图像为空，说明这是第一次获取图像，需要等待第一张图像到达
		while (Picture.empty())
		{
			lock.unlock();
			std::this_thread::yield();
			lock.lock();
		}
		IsPictureLatest = false;
		return Picture;
	}

	/// 将原始图像转化为矩阵
	cv::Mat BayerMatAcquisitor::ConvertRawDataToPicture(const AbstractAcquisitor::RawPicture &raw_picture)
	{
		auto picture = cv::Mat(cv::Size(raw_picture.Width, raw_picture.Height), CV_8UC1, raw_picture.Data);
		return picture;
	}
}