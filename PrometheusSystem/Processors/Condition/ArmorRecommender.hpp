#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>

namespace RoboPioneers::Prometheus::Processors
{
	/**
	 * @brief 装甲板推荐器
	 * @author Vincent
	 * @details
	 *  ~ 该过滤器用于从可能的装甲板中选择一个推荐。
	 */
	class ArmorRecommender AsProcessor
	{
	Requirement:
		using RotatedRectPair = std::tuple<cv::RotatedRect, cv::RotatedRect>;

		/// 裁剪偏移量
		Require(cv::Point, PositionOffset);
		/// 可能的装甲板列表
		Require(std::list<RotatedRectPair>, Armors);

		/// 裁剪目标区域
		Require(cv::Rect, CuttingArea);
		/// 指令位
		Require(char, Command);
		/// 横坐标
		Require(int, X);
		/// 纵坐标
		Require(int, Y);
		/// 数字
		Require(char, Number);

	public:
		cv::Point ScreenSize{1280, 1024};
		double HeightExpandRatio = 1.5f;
		double WidthExpandRatio = 1.5f;

		/// 最大追踪次数
		int MaxTrackingTimes {5};

		/// 当前是否正在追踪
		bool Tracking {false};
		/// 当前追踪剩余次数
		int TrackingRemainTimes {MaxTrackingTimes};



	public:
		/// 执行方法，将从装甲板列表中选择一个推荐，并提取裁剪区域和哼纵坐标
		void Execute() override;
	};
}