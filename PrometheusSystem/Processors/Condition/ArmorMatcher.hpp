#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <vector>
#include <list>
#include <tuple>

namespace RoboPioneers::Prometheus::Processors
{
	/**
	 * @brief 装甲板匹配器
	 * @author Vincent
	 * @details
	 *  ~ 该匹配器用于从可能的灯条矩形中尝试匹配装甲板。
	 */
	class ArmorMatcher AsProcessor
	{
	Requirement:
		/// 需要可能的灯条列表
		Require(std::list<cv::RotatedRect>, LightBars);

		using RotatedRectPair = std::tuple<cv::RotatedRect, cv::RotatedRect>;

		/// 可能的装甲板列表
		Require(std::list<RotatedRectPair>, Armors);

	public:
		/// 最大转角偏差值
		int MaxAngleDifference = 15;

		/// 最小Y坐标-高度比例，单位1%，该项用于显著消除噪点匹配和三灯条问题
		int MinDeltaYHeightRatio = 0;
		/// 最大Y坐标-高度比例，单位1%，该项用于显著消除噪点匹配和三灯条问题
		int MaxDeltaYHeightRatio = 30; //%

		/// 大装甲板的最小高度-距离比例，单位1%，该项用于避免灯条与血条上噪点匹配
		int MinHeightDistanceRatioBigArmor = 0;
		/// 大装甲板的最大高度-距离比例，单位1%，该项用于解决四灯条问题
		int MaxHeightDistanceRatioBigArmor = 59;
		/// 大装甲板的最小宽度-距离比例，单位1%，该项用于避免灯条与血条上噪点匹配
		int MinWidthDistanceRatioBigArmor = 15;
		/// 大装甲板的最小宽度-距离比例，单位1%，该项用于避免灯条与血条上噪点匹配
		int MaxWidthDistanceRatioBigArmor = 25;

		/// 小装甲板的最小高度-距离比例，单位1%
		int MinHeightDistanceRatioSmallArmor = 30;
		/// 小装甲板的最大高度-距离比例，单位1%
		int MaxHeightDistanceRatioSmallArmor = 100;
		/// 小装甲板的最小宽度-距离比例，单位1%
		int MinWidthDistanceRatioSmallArmor = 5;
		/// 小装甲板的最小宽度-距离比例，单位1%
		int MaxWidthDistanceRatioSmallArmor = 20;

	public:
		void Execute() override;
	};
}
