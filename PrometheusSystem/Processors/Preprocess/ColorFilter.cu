#include <opencv4/opencv2/core/cuda_stream_accessor.hpp>

/**
 * @brief 在GPU上运行的颜色筛选器
 * @param min_hue 色调最小值
 * @param max_hue 色调最大值
 * @param min_saturation 饱和度最小值
 * @param max_saturation 饱和度最大值
 * @param min_value 亮度最小值
 * @param max_value 亮度最大值
 * @param source 输入图像
 * @param target 目标二值图
 * @param width 输入图像的宽度
 * @param height 输入图像的高度
 */
__global__ void GPUFilterColor(
		int min_hue, int max_hue,
		int min_saturation, int max_saturation,
		int min_value, int max_value,
		cv::cuda::PtrStepSz<uchar3> source, cv::cuda::PtrStepSz<uchar1> target)
{
	const int id_x = static_cast<int>(blockIdx.x * blockDim.x + threadIdx.x);
	const int id_y = static_cast<int>(blockIdx.y * blockDim.y + threadIdx.y);

	if (id_x >= source.cols || id_y >= source.rows)
	{
		return;
	}

	uchar3 hsv = source(id_y, id_x);

	// 色域条件
	int hue_condition = (hsv.x  - min_hue) * (max_hue - hsv.x);
	// 饱和度条件
	int saturation_condition = (hsv.y - min_saturation) * (max_saturation - hsv.y);
	// 亮度条件
	int value_condition = (hsv.z - min_value) * (max_value - hsv.z);

	// 阈值化设置
	if (hue_condition >= 0 && value_condition >= 0 && saturation_condition >= 0)
	{
		target(id_y, id_x) = make_uchar1(255);
	}
	else
	{

		target(id_y, id_x) = make_uchar1(0);
	}
}

/**
 * @brief 过滤颜色
 * @param min_hue 色调最小值
 * @param max_hue 色调最大值
 * @parma min_saturation 饱和度最小值
 * @param max_saturation 饱和度最大值
 * @param min_value 亮度最小值
 * @param max_value 亮度最大值
 */
extern "C" void FilterColor(
		int min_hue, int max_hue,
		int min_saturation, int max_saturation,
		int min_value, int max_value,
		cv::cuda::GpuMat& source, cv::cuda::GpuMat& target, cv::cuda::Stream& stream)
{
	target = cv::cuda::GpuMat(source.size(), CV_8UC1, cv::Scalar(0,0,0));

	// 图形宽度
	const auto width = source.cols;
	// 图形高度
	const auto height = source.rows;

	// 每个Block要求16x16个线程
	dim3 threadsSize;
	threadsSize.x = 16;
	threadsSize.y = 16;

	// 根据图片大小计算Block个数
	dim3 blocksSize;
	blocksSize.x = width % threadsSize.x == 0 ? width / threadsSize.x : width / threadsSize.x + 1;
	blocksSize.y = height % threadsSize.y == 0 ? height / threadsSize.y : height / threadsSize.y + 1;

	GPUFilterColor<<<blocksSize, threadsSize, 0,  cv::cuda::StreamAccessor::getStream(stream)>>>(
			min_hue, max_hue, min_saturation, max_saturation, min_value, max_value,
			source, target);
}