#include "ArmorRecommender.hpp"

#include "../../Modules/GeometryFeatureModule.hpp"

#include <vector>

namespace RoboPioneers::Prometheus::Processors
{
	/// 执行方法
	void ArmorRecommender::Execute()
	{
		Command = 0;

		RotatedRectPair* best_one = nullptr;
		long best_score = -1;

		if (Armors.Acquire().empty())
		{
			// 若没找到，根据丢失技术判断是否维持裁剪区域
			Command = 0;

			if (Tracking)
			{
				// 若处于追踪状态，则减少追踪计数
				--TrackingRemainTimes;
			}
			if (TrackingRemainTimes <= 0)
			{
				// 追踪计数归零，则重置追踪状态
				Tracking = false;
				TrackingRemainTimes = MaxTrackingTimes;
			}

			if (!Tracking)
			{
				/// 清空裁剪区域
				auto& cutting_area = *CuttingArea;
				cutting_area.width = 0;
				cutting_area.height = 0;
			}

			return;
		}
		else
		{
			for (auto& pair : *Armors)
			{
				auto first_light = Modules::GeometryFeatureModule::StandardizeRotatedRectangle(std::get<0>(pair));
				auto second_light = Modules::GeometryFeatureModule::StandardizeRotatedRectangle(std::get<1>(pair));

				auto length = cv::norm(first_light.Center - second_light.Center);
				auto width = first_light.Length > second_light.Length ? first_light.Length : second_light.Length;

				auto center_point = (first_light.Center + second_light.Center) / 2;

				auto real_center_point = center_point + *PositionOffset;

				auto real_offset = cv::norm(real_center_point - (ScreenSize / 2));

				auto score = static_cast<long>(length * width / real_offset / real_offset);

				if (score > best_score)
				{
					best_one = &pair;
					best_score = score;
				}
			}

			auto& first_light = std::get<0>(*best_one);
			auto& second_light = std::get<1>(*best_one);

			auto center_point = (first_light.center + second_light.center) / 2;

			std::vector<cv::Point2f> armor_vertices;
			armor_vertices.resize(8);
			first_light.points(&armor_vertices[0]);
			second_light.points(&armor_vertices[4]);
			auto armor_rectangle = cv::minAreaRect(armor_vertices).boundingRect();

			auto& cutting_area = *CuttingArea;
			auto& global_offset = *PositionOffset;
			auto width = armor_rectangle.width;
			if (width < 100) width  = 100;
			auto height = armor_rectangle.height;
			if (height < 40) height = 40;
			cutting_area.x = center_point.x + global_offset.x - 0.5f * width * (WidthExpandRatio + 1);
			cutting_area.y = center_point.y + global_offset.y - 0.5f * height * (HeightExpandRatio + 1);
			cutting_area.width = width * (WidthExpandRatio + 1);
			cutting_area.height = height * (HeightExpandRatio + 1);

			// 暂时禁用裁剪
			cutting_area.x = 0;
			cutting_area.y = 0;
			cutting_area.width = 0;
			cutting_area.height = 0;

			*Command = 1;
			*X = center_point.x + global_offset.x;
			*Y = center_point.y + global_offset.y;
		}
	}
}