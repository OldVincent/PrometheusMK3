#include "LightBarsFilter.hpp"

#include "../../Modules/GeometryFeatureModule.hpp"

namespace RoboPioneers::Prometheus::Processors
{
	/// 执行方法
	void LightBarsFilter::Execute()
	{
		auto& contours = *Contours;
		auto& light_bars = *LightBars;
		light_bars.clear();

		for (auto& contour : contours)
		{
			auto area = cv::contourArea(contour);

			if (area < MinArea) continue;

			auto rotated_rectangle = cv::minAreaRect(contour);

			if (rotated_rectangle.size.area() <= 0) continue;

			if (area / rotated_rectangle.size.area() * 100 < MinFillingRatio) continue;

			light_bars.push_back(rotated_rectangle);
		}

	}
}