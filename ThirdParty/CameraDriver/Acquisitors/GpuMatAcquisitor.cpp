#ifndef NO_CUDA

#include "GpuMatAcquisitor.hpp"

#include <thread>

extern void CUDADeviceSynchronize();

namespace RoboPioneers::Modules::CameraDriver::Acquisitors
{
	/// 接收到图像事件
	void GpuMatAcquisitor::ReceivePictureIncomeEvent(AbstractAcquisitor::RawPicture data)
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
		auto&& gpu_picture = ConvertRawDataToGPUPicture(data, picture);

		std::unique_lock picture_lock(PictureMutex);
		Picture = std::move(picture);
		picture_lock.unlock();

		std::unique_lock gpu_picture_lock(GpuPictureMutex);
		GpuPicture = gpu_picture;
		gpu_picture_lock.unlock();
	}

	/// 获取GPU图像
	cv::cuda::GpuMat GpuMatAcquisitor::GetGpuPicture(bool wait_for_latest) noexcept(false)
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

		std::shared_lock lock(GpuPictureMutex);
		// 若图像为空，说明这是第一次获取图像，需要等待第一张图像到达
		while (GpuPicture.empty())
		{
			lock.unlock();
			std::this_thread::yield();
			lock.lock();
		}
		IsPictureLatest = false;
		return GpuPicture;
	}

	/// 将原始数据转换为显存中的图片
	cv::cuda::GpuMat GpuMatAcquisitor::ConvertRawDataToGPUPicture(
			const AbstractAcquisitor::RawPicture &data, const cv::Mat &picture)
	{
		cv::cuda::GpuMat gpu_mat(picture);
		CUDADeviceSynchronize();
		return gpu_mat;
	}
}

#endif