#include <opencv4/opencv2/opencv.hpp>
#include <ThirdParty/CameraDriver/CameraDriver.hpp>
#include <list>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "GeometryFeatureModule.hpp"
#include "ImageDebugUtility.hpp"

int MinHue = 4;
int MaxHue = 23;
int MinSaturation = 150;
int MaxSaturation = 255;
int MinValue = 90;
int MaxValue = 255;

int MinArea = 3;

int MinFillingRatio = 50; //%

int MaxAngleDifference = 15;

int MinDeltaYHeightRatio = 0; //%
int MaxDeltaYHeightRatio = 30; //%

int MinHeightDistanceRatioBigArmor = 0; //%
int MaxHeightDistanceRatioBigArmor = 59; //%
int MinWidthDistanceRatioBigArmor = 15; //%
int MaxWidthDistanceRatioBigArmor = 25; //%

int MinHeightDistanceRatioSmallArmor = 30; //%
int MaxHeightDistanceRatioSmallArmor = 100; //%
int MinWidthDistanceRatioSmallArmor = 5; //%
int MaxWidthDistanceRatioSmallArmor = 20; //%

int main()
{
	using namespace RoboPioneers::Modules;

	// 确保日志路径存在
	if(boost::filesystem::exists("Settings.json"))
	{
		try {
			boost::property_tree::ptree json_node;
			boost::property_tree::read_json("Settings.json", json_node);

			MinHue = json_node.get<int>("Mask.Red.Hue.Min");
			MaxHue = json_node.get<int>("Mask.Red.Hue.Max");
			MinSaturation = json_node.get<int>("Mask.Red.Saturation.Min");
			MaxSaturation = json_node.get<int>("Mask.Red.Saturation.Max");
			MinValue = json_node.get<int>("Mask.Red.Value.Min");
			MaxValue = json_node.get<int>("Mask.Red.Value.Max");

			MinArea = json_node.get<int>("LightBar.MinArea");
			MinFillingRatio = json_node.get<int>("LightBar.MinFillingRatio");
			MaxAngleDifference = json_node.get<int>("LightBar.MaxAngleDifference");
			MinDeltaYHeightRatio = json_node.get<int>("LightBar.DeltaYHeightRatio.Min");
			MaxDeltaYHeightRatio = json_node.get<int>("LightBar.DeltaYHeightRatio.Max");

			MinHeightDistanceRatioBigArmor = json_node.get<int>("BigArmor.HeightDistanceRatio.Min");
			MaxHeightDistanceRatioBigArmor = json_node.get<int>("BigArmor.HeightDistanceRatio.Max");
			MinWidthDistanceRatioBigArmor = json_node.get<int>("BigArmor.WidthDistanceRatio.Min");
			MaxWidthDistanceRatioBigArmor = json_node.get<int>("BigArmor.WidthDistanceRatio.Max");

			MinHeightDistanceRatioSmallArmor = json_node.get<int>("SmallArmor.HeightDistanceRatio.Min");
			MaxHeightDistanceRatioSmallArmor = json_node.get<int>("SmallArmor.HeightDistanceRatio.Max");
			MinWidthDistanceRatioSmallArmor = json_node.get<int>("SmallArmor.WidthDistanceRatio.Min");
			MaxWidthDistanceRatioSmallArmor = json_node.get<int>("SmallArmor.WidthDistanceRatio.Max");
		}
		catch(boost::wrapexcept<boost::property_tree::ptree_bad_path>& error)
		{
			std::cerr << "[Error] Incomplete Settings.json, Will Use Default Settings: " << error.what() << std::endl;
		}

	}

	CameraDriver::CameraDevice device;
	CameraDriver::Acquisitors::MatAcquisitor acquisitor(&device);

	device.Open(0);
	device.SetExposureTime(1000);
	device.SetGain(16);

	acquisitor.Start();

	cv::namedWindow("Raw Picture");
	cv::namedWindow("Mask");
	cv::namedWindow("Light Bars");

	cv::namedWindow("Armors Control");
	cv::resizeWindow("Armors Control", 800,400);

	cv::createTrackbar("Min Hue", "Mask", &MinHue, 180);
	cv::createTrackbar("Max Hue", "Mask", &MaxHue, 180);
	cv::createTrackbar("Min Saturation", "Mask", &MinSaturation, 255);
	cv::createTrackbar("Max Saturation", "Mask", &MaxSaturation, 255);
	cv::createTrackbar("Min Value", "Mask", &MinValue, 255);
	cv::createTrackbar("Max Value", "Mask", &MaxValue, 255);

	cv::createTrackbar("Min Area", "Light Bars", &MinArea, 1000);
	cv::createTrackbar("Filling Ratio (%)", "Light Bars", &MinFillingRatio, 100);

	cv::createTrackbar("Max Angle Difference", "Armors Control", &MaxAngleDifference, 180);
	cv::createTrackbar("Min Y-Height Ratio (%)", "Armors Control", &MinDeltaYHeightRatio, 100);
	cv::createTrackbar("Max Y-Height Ratio (%)", "Armors Control", &MaxDeltaYHeightRatio, 300);

	cv::createTrackbar("Min Height-Distance Ratio (%) (Big Armor)", "Armors Control", &MinHeightDistanceRatioBigArmor, 100);
	cv::createTrackbar("Max Height-Distance Ratio (%) (Big Armor)", "Armors Control", &MaxHeightDistanceRatioBigArmor, 300);
	cv::createTrackbar("Min Width-Distance Ratio (%) (Big Armor)", "Armors Control", &MinWidthDistanceRatioBigArmor, 100);
	cv::createTrackbar("Max Width-Distance Ratio (%) (Big Armor)", "Armors Control", &MaxWidthDistanceRatioBigArmor, 300);

	cv::createTrackbar("Min Height-Distance Ratio (%) (Small Armor)", "Armors Control", &MinHeightDistanceRatioSmallArmor, 100);
	cv::createTrackbar("Max Height-Distance Ratio (%) (Small Armor)", "Armors Control", &MaxHeightDistanceRatioSmallArmor, 300);
	cv::createTrackbar("Min Width-Distance Ratio (%) (Small Armor)", "Armors Control", &MinWidthDistanceRatioSmallArmor, 100);
	cv::createTrackbar("Max Width-Distance Ratio (%) (Small Armor)", "Armors Control", &MaxWidthDistanceRatioSmallArmor, 300);

	while(cv::waitKey(1) != 27)
	{
		auto bgr_picture = acquisitor.GetPicture(true);

		cv::resize(bgr_picture, bgr_picture, cv::Size(1024, 768));

		//------------------------------
		// 原始图像预处理
		//------------------------------

		cv::GaussianBlur(bgr_picture, bgr_picture, cv::Size(3,3), 0.8);

		cv::imshow("Raw Picture", bgr_picture);

		//------------------------------
		// 颜色过滤
		//------------------------------

		cv::Mat hsv_picture;
		cv::cvtColor(bgr_picture, hsv_picture, cv::COLOR_BGR2HSV);

		cv::Mat channels[3];
		cv::split(hsv_picture, channels);

		cv::Mat mask[6];
		cv::threshold(channels[0], mask[0], MinHue, 255, cv::THRESH_BINARY);
		cv::threshold(channels[0], mask[1], MaxHue, 255, cv::THRESH_BINARY_INV);

		cv::threshold(channels[1], mask[2], MinSaturation, 255, cv::THRESH_BINARY);
		cv::threshold(channels[1], mask[3], MaxSaturation, 255, cv::THRESH_BINARY_INV);

		cv::threshold(channels[2], mask[4], MinValue, 255, cv::THRESH_BINARY);
		cv::threshold(channels[2], mask[5], MaxValue, 255, cv::THRESH_BINARY_INV);

		cv::Mat result;
		cv::Mat min_mask, max_mask;
		cv::bitwise_and(mask[0], mask[2], min_mask, mask[4]);
		cv::bitwise_and(mask[1], mask[3], max_mask, mask[5]);
		cv::bitwise_and(min_mask, max_mask, result);

		cv::imshow("Mask", result);

		//------------------------------
		// 二值图像预处理
		//------------------------------

		cv::morphologyEx(result, result,cv::MORPH_CLOSE,
		                 cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3)));

		cv::imshow("After Preprocess", result);

		//==============================
		// 此处测试轮廓
		//==============================

		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(result, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		std::vector<std::vector<cv::Point>> filtered_contours;

		for (auto& contour : contours)
		{
			if (cv::contourArea(contour) >= MinArea)
			{
				filtered_contours.push_back(contour);
			}
		}

		cv::Mat background_picture(result.size(), CV_8UC3, cv::Scalar(0,0,0));
		cv::Mat foreground_picture(result.size(), CV_8UC3, cv::Scalar(255,255,255));
		cv::Mat light_bars_result_picture = background_picture;
		foreground_picture.copyTo(light_bars_result_picture, result);

		if (!filtered_contours.empty())
			cv::drawContours(light_bars_result_picture, filtered_contours, -1, cv::Scalar(0, 255, 255), cv::FILLED);

		//==============================
		// 此处测试灯条
		//==============================

		std::list<cv::RotatedRect> filtered_rectangles;

		for (auto filtered_contour : filtered_contours)
		{
			auto rotated_rectangle = cv::minAreaRect(filtered_contour);
			auto geometry_feature = GeometryFeatureModule::StandardizeRotatedRectangle(rotated_rectangle);

			if (cv::contourArea(filtered_contour) / rotated_rectangle.size.area() * 100 < MinFillingRatio) continue;

			filtered_rectangles.push_back(rotated_rectangle);
		}

		for (auto& filtered_rotated_rectangle : filtered_rectangles)
		{
			ImageDebugUtility::DrawRotatedRectangle(light_bars_result_picture, filtered_rotated_rectangle,
			                                        cv::Scalar(0,252,124), 3);
		}

		cv::imshow("Light Bars", light_bars_result_picture);

		//==============================
		// 此处测试装甲板匹配
		//==============================

		std::list<std::tuple<cv::RotatedRect, cv::RotatedRect>> matched_armors;

		for (auto main_index = filtered_rectangles.begin(); main_index != filtered_rectangles.end(); ++main_index)
		{
			auto vice_index = main_index;
			++vice_index;

			auto main_geometry_feature =  GeometryFeatureModule::StandardizeRotatedRectangle(*main_index);

			for (; vice_index != filtered_rectangles.end(); ++vice_index)
			{
				auto vice_geometry_feature = GeometryFeatureModule::StandardizeRotatedRectangle(*vice_index);

				//------------------------------
				// 角度之差
				//------------------------------

				auto angle_difference = main_geometry_feature.Angle - vice_geometry_feature.Angle;
				if (angle_difference < 0) angle_difference = -angle_difference;

				if (angle_difference > MaxAngleDifference) continue;

				//------------------------------
				// Y坐标差值-高度比
				//------------------------------

				auto height = main_geometry_feature.Length > vice_geometry_feature.Length ?
				              main_geometry_feature.Length : vice_geometry_feature.Length;

				auto delta_y = main_geometry_feature.Center.y - vice_geometry_feature.Center.y;
				if (delta_y < 0) delta_y = -delta_y;

				auto delta_y_height_ratio = delta_y / height * 100;
				if (delta_y_height_ratio > MaxDeltaYHeightRatio || delta_y_height_ratio < MinDeltaYHeightRatio)
					continue;

				//------------------------------
				// 高度-距离比
				//------------------------------

				auto distance = cv::norm(main_geometry_feature.Center - vice_geometry_feature.Center);

				auto height_distance_ratio = height / distance * 100;
				auto big_armor_hd_matched = height_distance_ratio < MaxHeightDistanceRatioBigArmor && height_distance_ratio > MinHeightDistanceRatioBigArmor;
				auto small_armor_hd_matched = height_distance_ratio < MaxHeightDistanceRatioSmallArmor && height_distance_ratio > MinHeightDistanceRatioSmallArmor;

				if (!big_armor_hd_matched && !small_armor_hd_matched) continue;

				//------------------------------
				// 宽度-距离比
				//------------------------------

				auto width = main_geometry_feature.Width > vice_geometry_feature.Width ?
				             main_geometry_feature.Width : vice_geometry_feature.Width;
				auto width_distance_ratio = width / distance * 100;
				auto big_armor_wd_matched = width_distance_ratio < MaxWidthDistanceRatioBigArmor && width_distance_ratio > MinWidthDistanceRatioBigArmor;
				auto small_armor_wd_matched = width_distance_ratio < MaxWidthDistanceRatioSmallArmor && width_distance_ratio > MinWidthDistanceRatioSmallArmor;

				if (!big_armor_wd_matched && !small_armor_wd_matched) continue;

				matched_armors.emplace_back(*main_index, *vice_index);
			}
		}

		cv::Mat armors_result_picture(bgr_picture.size(), CV_8UC3);
		armors_result_picture = bgr_picture.clone();

		for (auto& armor : matched_armors)
		{
			cv::line(armors_result_picture, std::get<0>(armor).center, std::get<1>(armor).center,
				cv::Scalar(0,255,0),2);
		}

		cv::imshow("Armors", armors_result_picture);
	}

	// 保存设定
	{
		boost::property_tree::ptree json_node;

		json_node.put<int>("Mask.Red.Hue.Min", MinHue);
		json_node.put<int>("Mask.Red.Hue.Max", MaxHue);
		json_node.put<int>("Mask.Red.Saturation.Min", MinSaturation);
		json_node.put<int>("Mask.Red.Saturation.Max", MaxSaturation);
		json_node.put<int>("Mask.Red.Value.Min", MinValue);
		json_node.put<int>("Mask.Red.Value.Max", MaxValue);

		json_node.put<int>("LightBar.MinArea", MinArea);
		json_node.put<int>("LightBar.MinFillingRatio", MinFillingRatio);
		json_node.put<int>("LightBar.MaxAngleDifference", MaxAngleDifference);
		json_node.put<int>("LightBar.DeltaYHeightRatio.Min", MinDeltaYHeightRatio);
		json_node.put<int>("LightBar.DeltaYHeightRatio.Max", MaxDeltaYHeightRatio);

		json_node.put<int>("BigArmor.HeightDistanceRatio.Min", MinHeightDistanceRatioBigArmor);
		json_node.put<int>("BigArmor.HeightDistanceRatio.Max", MaxHeightDistanceRatioBigArmor);
		json_node.put<int>("BigArmor.WidthDistanceRatio.Min", MinWidthDistanceRatioBigArmor);
		json_node.put<int>("BigArmor.WidthDistanceRatio.Max", MaxWidthDistanceRatioBigArmor);

		json_node.put<int>("SmallArmor.HeightDistanceRatio.Min", MinHeightDistanceRatioSmallArmor);
		json_node.put<int>("SmallArmor.HeightDistanceRatio.Max", MaxHeightDistanceRatioSmallArmor);
		json_node.put<int>("SmallArmor.WidthDistanceRatio.Min", MinWidthDistanceRatioSmallArmor);
		json_node.put<int>("SmallArmor.WidthDistanceRatio.Max", MaxWidthDistanceRatioSmallArmor);

		boost::property_tree::write_json("Settings.json", json_node);
	}

	acquisitor.Stop();
	device.Close();
}