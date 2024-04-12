#include "RawAcquisitor.hpp"

#include <thread>

namespace RoboPioneers::Modules::CameraDriver::Acquisitors
{
	/// 接收到图片事件
	void RawAcquisitor::ReceivePictureIncomeEvent(AbstractAcquisitor::RawPicture data)
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

		std::unique_lock lock(PictureMutex);
		Picture = data;
	}

	/// 获取图片方法
	AbstractAcquisitor::RawPicture RawAcquisitor::GetPicture(bool wait_for_latest) noexcept(false)
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
		while (!Picture.Data)
		{
			lock.unlock();
			std::this_thread::yield();
			lock.lock();
		}
		IsPictureLatest = false;

		return Picture;
	}
}
