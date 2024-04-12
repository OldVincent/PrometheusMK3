#include "ColorFilter.hpp"

/// 过滤颜色
extern "C" void FilterColor(
		int min_hue, int max_hue,
		int min_saturation, int max_saturation,
		int min_value, int max_value,
		cv::cuda::GpuMat& source, cv::cuda::GpuMat& target, cv::cuda::Stream& stream);

namespace RoboPioneers::Prometheus::Processors
{
	/// 执行方法，将过滤指定颜色
	void ColorFilter::Execute()
	{
		if (TargetColor == TargetColorType::Red)
		{
			// 应用红色选择器
			FilterColor(ColorForRed.MinHue, ColorForRed.MaxHue,
			   ColorForRed.MinSaturation, ColorForRed.MaxSaturation,
			   ColorForRed.MinValue, ColorForRed.MaxValue,
			   *FromGpuPicture, *ToGpuPicture, *GpuStream);
		}
		else
		{
			// 应用蓝色选择器
			FilterColor(ColorForBlue.MinHue, ColorForBlue.MaxHue,
			            ColorForBlue.MinSaturation, ColorForBlue.MaxSaturation,
			            ColorForBlue.MinValue, ColorForBlue.MaxValue,
			            *FromGpuPicture, *ToGpuPicture, *GpuStream);
		}
	}
}
