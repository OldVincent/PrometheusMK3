#include "PictureAcquirer.hpp"
#include <iostream>
#include <thread>
#include <mutex>

namespace RoboPioneers::Prometheus::Processors
{
	/// 设备静态对象自动关闭脚本
	class CameraObjectsManagerScript
	{
	private:
		bool IsOpened {false};
		std::mutex OpenMutex;

	public:
		/// 设备对象
		Modules::CameraDriver::CameraDevice Device;
		/// 采集器对象
		Modules::CameraDriver::Acquisitors::BayerMatAcquisitor Acquisitor {&Device};

		unsigned int CameraExposureMicroSeconds = 1000;
		unsigned int CameraGain = 16;
		unsigned int WaitingSeconds = 30;

		CameraObjectsManagerScript() = default;

		/// 析构函数，将自动关闭设备和采集器
		~CameraObjectsManagerScript()
		{
			if (Acquisitor.IsWorking())
			{
				Acquisitor.Stop();
			}
			if (Device.IsOpened())
			{
				Device.Close();
			}
		}

		/// 若设备未打开在，则打开设备
		void Open(unsigned int index)
		{
			std::unique_lock lock(OpenMutex, std::try_to_lock);
			if (!lock.owns_lock())
				return;
			if (!IsOpened)
			{
				bool device_opened = false;
				while (!device_opened)
				{
					if (!Device.IsOpened())
					{
						try
						{
							Acquisitor.GetDevice()->Open(index);
							device_opened = true;
							Acquisitor.Start();

							Acquisitor.GetDevice()->SetExposureTime(CameraExposureMicroSeconds);
							Acquisitor.GetDevice()->SetGain(CameraGain);

							Acquisitor.GetDevice()->SetWhiteBalance(
									Modules::CameraDriver::CameraDevice::WhiteBalanceChannel::Red, 1.2344);
							Acquisitor.GetDevice()->SetWhiteBalance(
									Modules::CameraDriver::CameraDevice::WhiteBalanceChannel::Blue, 1.4258);
						}catch (std::runtime_error& error)
						{
							device_opened = false;

							std::cerr << "[Error] Failed to Open Camera Device: " << error.what() << std::endl;
							std::clog << "[Log] Camera Device Retry will Happen in 30 Seconds. " << std::endl;

							std::this_thread::sleep_for(std::chrono::seconds(WaitingSeconds));
						}
					}
				}
				if (!Acquisitor.IsWorking())
				{
					Acquisitor.Start();
				}
				IsOpened = true;
			}
		}
	};

	CameraObjectsManagerScript* GetManagedCameraObjects()
	{
		static CameraObjectsManagerScript manager;
		return &manager;
	}

	/// 执行方法
	void PictureAcquirer::Execute()
	{
		if (!GetManagedCameraObjects()->Acquisitor.IsWorking())
		{
			OnInitialize();
		}
		Picture.Set(GetManagedCameraObjects()->Acquisitor.GetPicture(WaitForLatest));
	}

	/// 初始化方法
	void PictureAcquirer::OnInitialize()
	{
		GetManagedCameraObjects()->WaitingSeconds = WaitingSeconds;
		GetManagedCameraObjects()->CameraExposureMicroSeconds = CameraExposureMicroSeconds;
		GetManagedCameraObjects()->CameraGain = CameraGain;
		GetManagedCameraObjects()->Open(CameraIndex);
	}

	/// 终止化方法
	void PictureAcquirer::OnFinalize()
	{
		GetManagedCameraObjects()->Acquisitor.Stop();
	}
}
