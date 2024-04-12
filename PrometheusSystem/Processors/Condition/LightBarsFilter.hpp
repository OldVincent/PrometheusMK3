#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <opencv4/opencv2/opencv.hpp>

#include <vector>
#include <list>
#include <tuple>

namespace RoboPioneers::Prometheus::Processors
{
	/**
	 * @brief 灯条过滤器
	 * @author Vincent
	 * @details
	 *  ~ 该流处理器用于根据几何条件过滤矩形，筛选出可能的灯条。
	 */
	class LightBarsFilter AsProcessor
	{
	Requirement:
		/// 轮廓集合
		Require(std::vector<std::vector<cv::Point>>, Contours);
		/// 灯条列表
		Require(std::list<cv::RotatedRect>, LightBars);

	public:
		/**
		 * @brief 最小面积
		 * @details
		 *  ~ 规定了轮廓进入候选列表的最小面积。
		 *  ~ 该项用于剔除绝大多数噪点，但同时会限制最远工作距离。
		 */
		int MinArea {3};
		/**
		 * @brief 最小填充率
		 * @details
		 *  ~ 规定了轮廓的面积占外接矩形的比例的最小百分比。
		 *  ~ 该项用于剔除不规则的噪点。
		 */
		int MinFillingRatio {50};

	public:
		/// 执行方法
		void Execute() override;
	};
}