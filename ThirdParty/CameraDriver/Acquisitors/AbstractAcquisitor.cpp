#include "AbstractAcquisitor.hpp"

#include <GxIAPI.h>
#include <stdexcept>

/// 相机获取图像回调
void AcquisitorCameraCaptureCallback(GX_FRAME_CALLBACK_PARAM* parameter)
{
	using namespace RoboPioneers::Modules::CameraDriver::Acquisitors;

	auto* target = static_cast<AbstractAcquisitor*>(parameter->pUserParam);
	if (target)
	{
		target->ReceivePictureIncomeEvent(AbstractAcquisitor::RawPicture{
			const_cast<void *>(parameter->pImgBuf),
			parameter->nWidth, parameter->nHeight});
	}
}

/// 相机离线回调
void CameraOfflineCallback(void *pUserParam)
{
	using namespace RoboPioneers::Modules::CameraDriver::Acquisitors;

	auto* target = static_cast<AbstractAcquisitor*>(pUserParam);
	if (target)
	{
		target->ReceiveDeviceOfflineEvent();
	}
}

namespace RoboPioneers::Modules::CameraDriver::Acquisitors
{
	/// 构造函数，将绑定相机设备
	AbstractAcquisitor::AbstractAcquisitor(CameraDevice *device) : Device(device)
	{}

	/// 析构函数
	AbstractAcquisitor::~AbstractAcquisitor()
	{
		if (Working)
		{
			GX_STATUS operation_result;
			// 发送命令停止采集
			GXSendCommand(this->GetDevice()->GetDeviceHandle(), GX_COMMAND_ACQUISITION_START);
			// 注销采集事件
			GXUnregisterCaptureCallback(this->GetDevice()->GetDeviceHandle());
			// 注销设备离线事件
			GXUnregisterDeviceOfflineCallback(this->GetDevice()->GetDeviceHandle(), OfflineEventHandle);

			Working = false;
		}
	}

	/// 开始采集
	void AbstractAcquisitor::Start()
	{
		if (Working)
		{
			return;
		}

		// 核验设备指针及设备状态
		if (!GetDevice())
		{
			throw std::runtime_error("MatAcquisitor::Start Camera Device Pointer is Null");
		}
		if (!GetDevice()->GetDeviceHandle())
		{
			throw std::runtime_error("MatAcquisitor::Start Camera Device Handle Pointer is Null");
		}
		if (!GetDevice()->IsOpened())
		{
			throw std::runtime_error("MatAcquisitor::Start Camera Device Has Not Been Opened.");
		}

		GX_STATUS operation_result;

		// 注册采集回调
		operation_result = GXRegisterCaptureCallback(this->GetDevice()->GetDeviceHandle(),
		                                             this, AcquisitorCameraCaptureCallback);
		if (operation_result != GX_STATUS_LIST::GX_STATUS_SUCCESS)
		{
			throw std::runtime_error("MatAcquisitor::Start Failed to Register Capture Callback.");
		}

		// 注册离线回调
		operation_result = GXRegisterDeviceOfflineCallback(this->GetDevice()->GetDeviceHandle(),
		                                                   this, CameraOfflineCallback, &OfflineEventHandle);
		if (operation_result != GX_STATUS_LIST::GX_STATUS_SUCCESS)
		{
			throw std::runtime_error("MatAcquisitor::Start Failed to Register Offline Callback.");
		}

		// 发送命令开始采集
		operation_result = GXSendCommand(this->GetDevice()->GetDeviceHandle(),
		                                 GX_COMMAND_ACQUISITION_START);
		if (operation_result != GX_STATUS_LIST::GX_STATUS_SUCCESS)
		{
			throw std::runtime_error("MatAcquisitor::Start Failed to Start Acquisition.");
		}

		// 更新采集器状态
		Working = true;
	}

	/// 停止采集
	void AbstractAcquisitor::Stop()
	{
		// 核验设备指针
		if (!GetDevice())
		{
			throw std::runtime_error("MatAcquisitor::Stop Camera Device Pointer is Null");
		}
		if (!GetDevice()->GetDeviceHandle())
		{
			throw std::runtime_error("MatAcquisitor::Stop Camera Device Handle Pointer is Null");
		}

		GX_STATUS operation_result;
		// 发送命令停止采集
		GXSendCommand(this->GetDevice()->GetDeviceHandle(), GX_COMMAND_ACQUISITION_START);
		// 注销采集事件
		GXUnregisterCaptureCallback(this->GetDevice()->GetDeviceHandle());
		// 注销设备离线事件
		GXUnregisterDeviceOfflineCallback(this->GetDevice()->GetDeviceHandle(), OfflineEventHandle);

		Working = false;
	}

	/// 调用停止方法
	void AbstractAcquisitor::ReceiveDeviceOfflineEvent()
	{
		Working = false;
	}
}