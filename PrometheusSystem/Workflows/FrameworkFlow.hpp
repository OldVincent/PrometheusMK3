#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <shared_mutex>
#include <condition_variable>

#include <iostream>

#ifdef DEBUG
#include "../Modules/ImageDebugUtility.hpp"
#endif
#include "../Modules/GeometryFeatureModule.hpp"

#include "../Processors/Transimission/PictureAcquirer.hpp"
#include "../Processors/Transimission/GpuPictureUploader.hpp"
#include "../Processors/Transimission/GpuPictureDownloader.hpp"
#include "../Processors/Transimission/SerialCommand.hpp"
#include "../Processors/Transimission/WaitGPUStream.hpp"

#include "../Processors/Preprocess/PictureCutter.hpp"
#include "../Processors/Preprocess/BayerBGToHSVConverter.hpp"
#include "../Processors/Preprocess/ColorFilter.hpp"
#include "../Processors/Preprocess/GaussBlurProcessor.hpp"
#include "../Processors/Preprocess/OpenBlurProcessor.hpp"
#include "../Processors/Preprocess/CloseBlurProcessor.hpp"

#include "../Processors/Detection/ContoursDetector.hpp"
#include "../Processors/Condition/LightBarsFilter.hpp"
#include "../Processors/Condition/ArmorMatcher.hpp"
#include "../Processors/Condition/ArmorRecommender.hpp"

#include "../Processors/Debug/MatView.hpp"
#include "../Processors/Debug/GpuMatView.hpp"

namespace RoboPioneers::Prometheus
{
	/**
	 * @brief 帧工作流
	 * @author Vincent
	 * @details
	 *  ~ 该工作流对应一帧。
	 */
	class FrameworkFlow AsWorkflow
	{
	Executors:
		NeedParallelExecutor(MultiCores) {};
		NeedSerialExecutor(MainCore) {};
		NeedSerialExecutor(ViceCore) {};

	Channels:
		//==============================
		// 预处理部分
		//==============================

		/// 显卡处理流
		Galaxy::Channel<cv::cuda::Stream> GpuStream Provide("GpuStream");
		/// 相机图片通道
		Galaxy::Channel<cv::Mat> Picture Provide("Picture", "CuttingPicture");
		/// 裁剪区域通道
		Galaxy::Channel<cv::Rect> CuttingArea Provide({0, 0, 0, 0},"CuttingArea");
		/// 全局坐标偏移
		Galaxy::Channel<cv::Point> PositionOffset Provide("PositionOffset");

		/// GPU原始图片通道
		Galaxy::Channel<cv::cuda::GpuMat> GpuPicture Provide("GpuPicture");
		/// GPU模糊彩色图片通道
		Galaxy::Channel<cv::cuda::GpuMat> GpuBlurPicture Provide(
				cv::cuda::GpuMat(cv::Size(1280,1024), CV_8UC3), "GpuBlurPicture");

		/// 二值GPU图片通道
		Galaxy::Channel<cv::cuda::GpuMat> GpuBinaryPicture Provide("GpuBinaryPicture");
		/// 二值图片通道
		Galaxy::Channel<cv::Mat> BinaryPicture Provide("BinaryPicture");

		/// 轮廓列表
		Galaxy::Channel<std::vector<std::vector<cv::Point2i>>> Contours Provide("Contours", {});
		/// 灯条列表
		Galaxy::Channel<std::list<cv::RotatedRect>> LightBars Provide("LightBars", {});
		/// 装甲板列表
		Galaxy::Channel<std::list<std::tuple<cv::RotatedRect, cv::RotatedRect>>> Armors Provide("Armors", {});

		/// 指令通道
		Galaxy::Channel<char> Command Provide("Command");
		/// 横坐标
		Galaxy::Channel<int> X Provide("X");
		/// 纵坐标
		Galaxy::Channel<int> Y Provide("Y");
		/// 数字识别
		Galaxy::Channel<char> Number Provide("Number");

	Procedure:
		//==============================
		// 预处理部分
		//==============================

		/// 获取Bayer格式图片
		Processors::PictureAcquirer OriginalPictureAcquirer On(MultiCores);

		/// 裁剪图片
		Processors::PictureCutter InterestedAreaCutter On(MultiCores);
		/// 上传图片
		Processors::GpuPictureUploader RawPictureUploader On(MultiCores);

		/// 转换颜色格式
		Processors::BayerBGToHSVConverter ConvertToHSV On(MultiCores, "GpuPicture", "GpuPicture");

		/// 高斯模糊
		Processors::GaussBlurProcessor GaussBlur On(MultiCores, CV_8UC3, "GpuPicture", "GpuBlurPicture");

		/// 颜色感知
		Processors::ColorFilter ColorPerception On(MultiCores, Processors::ColorFilter::TargetColorType::Red,
		                                           "GpuPicture", "GpuBinaryPicture");

		/// 开运算处理
		Processors::CloseBlurProcessor CloseBlur On(MultiCores, CV_8UC1, 3, "GpuBinaryPicture", "GpuBinaryPicture");
		/// 将二值图从显存下载到内存
		Processors::GpuPictureDownloader BinaryPictureDownloader On(MultiCores, "GpuBinaryPicture", "BinaryPicture");

		Processors::WaitGPUStream WaitBinaryPictureDownload On(MultiCores);

		/// 轮廓识别器
		Processors::ContoursDetector DetectContours On(ViceCore);

		/// 灯条检测器
		Processors::LightBarsFilter FilterLightBars On(MainCore);

		#ifdef DEBUG

		Galaxy::BuiltIn::LambdaAction DrawLightBars On(MainCore, [
				channel_gpu_picture = &this->GpuPicture,
				channel_light_bars = &this->LightBars]{
			cv::Mat picture;
			cv::cuda::Stream stream;
			channel_gpu_picture->Acquire().download(picture, stream);
			stream.waitForCompletion();
			for (auto& light_bar : channel_light_bars->Acquire())
			{
				Modules::ImageDebugUtility::DrawRotatedRectangle(picture, light_bar,
													 cv::Scalar(0,255,0), 3);
			}
			cv::imshow("LightBars", picture);
		});

		#endif

		/// 装甲板识别
		Processors::ArmorMatcher MatchArmors On(MainCore);
		/// 装甲板推荐
		Processors::ArmorRecommender RecommendArmor On(MainCore);

		Galaxy::BuiltIn::LambdaAction PrintPosition On(MainCore, [
				channel_command = &this->Command,
				channel_x = &this->X,
				channel_y = &this->Y]{
			if (channel_command->Acquire() == 1)
			{
				std::cout << "Found X:" << channel_x->Acquire() << " Y:" << channel_y->Acquire() << std::endl;
			}
		});

		#ifndef DEBUG
		Processors::SerialCommand SendSerialCommand On(MultiCores, "/dev/ttyTHS2");
		#endif

		Galaxy::BuiltIn::LambdaAction ThirdStageEndNotifier On(MainCore,[]{});
	};
}