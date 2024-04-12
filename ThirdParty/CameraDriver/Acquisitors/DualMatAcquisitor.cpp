#include "DualMatAcquisitor.hpp"

#ifndef NO_CUDA

#include <thread>

extern void CUDADeviceSynchronize();

namespace RoboPioneers::Modules::CameraDriver::Acquisitors
{
	/// 获取图片
	std::tuple<cv::Mat, cv::cuda::GpuMat> DualMatAcquisitor::GetDualPicture(bool wait_for_latest)
	{
		if (!IsWorking())
		{
			throw std::runtime_error("DualMatAcquisitor::GetPicture Device Is Offline.");
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
		return {Picture, GpuPicture};
	}
}

#endif
