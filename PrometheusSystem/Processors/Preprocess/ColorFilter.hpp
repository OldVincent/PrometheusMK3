#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>

namespace RoboPioneers::Prometheus::Processors
{
	/**
	 * @brief 颜色过滤器
	 * @author Vincent
	 * @details
	 *  ~ 使用BinaryGpuPicture作为输入输出通道。
	 */
	class ColorFilter AsProcessor
	{
	Requirement:
		Require(cv::cuda::GpuMat, FromGpuPicture);
		Require(cv::cuda::GpuMat, ToGpuPicture);
		RequireOptional(cv::cuda::Stream, GpuStream);

	public:
		/// 目标颜色类型
		enum class TargetColorType
		{
			Red,
			Blue
		} TargetColor;

		/// 颜色设定
		struct ColorSettings
		{
			/// 色调最小值
			int MinHue;
			/// 色调最大值
			int MaxHue;
			/// 饱和度最小值
			int MinSaturation;
			/// 饱和度最大值
			int MaxSaturation;
			/// 亮度最小值
			int MinValue;
			/// 亮度最大值
			int MaxValue;
		};

		/// 红色的匹配设定
		ColorSettings ColorForRed{
			.MinHue = 4,
			.MaxHue = 23,
			.MinSaturation = 150,
			.MaxSaturation = 255,
			.MinValue = 90,
			.MaxValue = 255
		};

		/// 蓝色的匹配设定
		ColorSettings ColorForBlue{
			.MinHue = 4,
			.MaxHue = 23,
			.MinSaturation = 150,
			.MaxSaturation = 255,
			.MinValue = 90,
			.MaxValue = 255
		};

	public:
		/// 配置方法
		Configure(ColorFilter, TargetColorType target_color,
			Name(FromGpuPicture), Name(ToGpuPicture), OptionalName(GpuStream)),
			TargetColor(target_color)
		{
			ApplyName(FromGpuPicture);
			ApplyName(ToGpuPicture);
			ApplyName(GpuStream);
		}

		/// 执行方法
		void Execute() override;
	};
}