#include "Controller.hpp"

#include <iostream>
#include <thread>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace RoboPioneers::Prometheus
{
	/// 启动方法
	void Controller::Launch()
	{
		//==============================
		// 准备工作流
		//==============================

		for (int index = 0; index < Frames.size(); ++index)
		{
			auto& frame = Frames[index];

			frame->Loop = true;
			frame->MultiCores = MultiCores;
			frame->MainCore = MainCore;
			frame->ViceCore = ViceCore;

			frame->ThirdStageEndNotifier.Operation = [this, index]{
				this->OnFrameThirdStageFinished(index);
			};

			LoadSettings(frame);

			//==============================
			// 提交工作流
			//==============================

			MultiCores->Submit(Frames[index]);

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		//==============================
		// 注册、启动并阻塞执行器
		//==============================

		Galaxy::Runtime::GetInstance()->RegisterExecutors({&A57, &Denver1, &Denver2});

		A57.Start();
		Denver1.Start();
		Denver2.Start();

		A57.Join();
		Denver1.Join();
		Denver2.Join();
	}

	void Controller::LoadSettings(FrameworkFlow* frame)
	{
		// 确保日志路径存在
		if(boost::filesystem::exists("Settings.json"))
		{
			boost::property_tree::ptree json_node;
			boost::property_tree::read_json("Settings.json", json_node);

			frame->ColorPerception.ColorForRed.MinHue = json_node.get<int>("Mask.Red.Hue.Min");
			frame->ColorPerception.ColorForRed.MaxHue = json_node.get<int>("Mask.Red.Hue.Max");
			frame->ColorPerception.ColorForRed.MinSaturation = json_node.get<int>("Mask.Red.Saturation.Min");
			frame->ColorPerception.ColorForRed.MaxSaturation = json_node.get<int>("Mask.Red.Saturation.Max");
			frame->ColorPerception.ColorForRed.MinValue = json_node.get<int>("Mask.Red.Value.Min");
			frame->ColorPerception.ColorForRed.MaxValue = json_node.get<int>("Mask.Red.Value.Max");

			frame->FilterLightBars.MinArea = json_node.get<int>("LightBar.MinArea");
			frame->FilterLightBars.MinFillingRatio = json_node.get<int>("LightBar.MinFillingRatio");
			frame->MatchArmors.MaxAngleDifference = json_node.get<int>("LightBar.MaxAngleDifference");
			frame->MatchArmors.MinDeltaYHeightRatio = json_node.get<int>("LightBar.DeltaYHeightRatio.Min");
			frame->MatchArmors.MaxDeltaYHeightRatio = json_node.get<int>("LightBar.DeltaYHeightRatio.Max");

			frame->MatchArmors.MinHeightDistanceRatioBigArmor = json_node.get<int>("BigArmor.HeightDistanceRatio.Min");
			frame->MatchArmors.MaxHeightDistanceRatioBigArmor = json_node.get<int>("BigArmor.HeightDistanceRatio.Max");
			frame->MatchArmors.MinWidthDistanceRatioBigArmor = json_node.get<int>("BigArmor.WidthDistanceRatio.Min");
			frame->MatchArmors.MaxWidthDistanceRatioBigArmor = json_node.get<int>("BigArmor.WidthDistanceRatio.Max");

			frame->MatchArmors.MinHeightDistanceRatioSmallArmor = json_node.get<int>("SmallArmor.HeightDistanceRatio.Min");
			frame->MatchArmors.MaxHeightDistanceRatioSmallArmor = json_node.get<int>("SmallArmor.HeightDistanceRatio.Max");
			frame->MatchArmors.MinWidthDistanceRatioSmallArmor = json_node.get<int>("SmallArmor.WidthDistanceRatio.Min");
			frame->MatchArmors.MaxWidthDistanceRatioSmallArmor = json_node.get<int>("SmallArmor.WidthDistanceRatio.Max");

			std::clog << "[Message] Using Settings in Settings.json." << std::endl;
		}
	}

	void Controller::OnFrameFirstStageFinished(unsigned int frame_index)
	{
//		unsigned int next_index = (frame_index + 1) % Frames.size();
//		MultiCores->Submit(Frames[next_index]);
	}

	void Controller::OnFrameThirdStageFinished(unsigned int frame_index)
	{
		++FramesCount;

		auto current_time = std::chrono::steady_clock::now();

		if (std::chrono::duration_cast<std::chrono::seconds>(current_time - FrameLastRecordTime).count() >= 1)
		{
			FrameLastRecordTime = current_time;
			std::cout << "FPS: " << FramesCount << std::endl;
			FramesCount = 0;
		}

		if (cv::waitKey(1) == 27)
		{
			Galaxy::Runtime::GetInstance()->StopAllExecutors();
		}
	}
}