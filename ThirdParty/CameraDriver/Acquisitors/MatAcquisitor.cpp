#include "MatAcquisitor.hpp"

#include <DxImageProc.h>

namespace RoboPioneers::Modules::CameraDriver::Acquisitors
{
	/// 将原始图像转化为矩阵
	cv::Mat MatAcquisitor::ConvertRawDataToPicture(const AbstractAcquisitor::RawPicture &data)
	{
		auto picture = cv::Mat(cv::Size(data.Width, data.Height), CV_8UC3);
		DxRaw8toRGB24(const_cast<void *>(data.Data), picture.data,
		              static_cast<VxUint32>(data.Width), static_cast<VxUint32>(data.Height),
		              RAW2RGB_NEIGHBOUR, BAYERBG, false);
		return picture;
	}
}