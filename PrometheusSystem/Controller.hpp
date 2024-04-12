#pragma once

#include <GalaxyEngine/GalaxyEngine.hpp>
#include <chrono>
#include <vector>
#include <atomic>
#include <tbb/tbb.h>
#include "Workflows/FrameworkFlow.hpp"

namespace RoboPioneers::Prometheus
{
	/**
	 * @brief 控制器类
	 * @author Vincent
	 * @details
	 *  ~ 该类用于控制Promethues攻击系统的整体活动。
	 */
	class Controller
	{
	protected:
		/// A57 四核处理器
		Galaxy::ParallelExecutor A57 {0,3,4,5};
		/// 丹佛大核处理器1号
		Galaxy::SerialExecutor Denver1 {1};
		/// 丹佛大核处理器2号
		Galaxy::SerialExecutor Denver2 {2};

		/// 多核指针
		Galaxy::ParallelExecutor* MultiCores{&A57};
		/// 主大核的指针
		Galaxy::SerialExecutor* MainCore{&Denver1};
		/// 副大核的指针
		Galaxy::SerialExecutor* ViceCore{&Denver2};

		/// 第一工作流
		FrameworkFlow FirstFrame;
		/// 第二工作流
		FrameworkFlow SecondFrame;
		/// 第三工作流
		FrameworkFlow ThirdFrame;

	private:
		/// 上次记录时间
		std::chrono::steady_clock::time_point FrameLastRecordTime;
		/// 从上次记录时间开始经过的帧数
		unsigned int FramesCount {0};

	protected:
		/**
		 * @brief 帧队列
		 * @details 注意，开了帧重叠反而是负优化
		 */
		std::vector<FrameworkFlow*> Frames {&FirstFrame};

	public:
		/// 从配置文件中加载设定
		void LoadSettings(FrameworkFlow* frame);

		/// 帧第一阶段结束事件
		virtual void OnFrameFirstStageFinished(unsigned int frame_index);

		/// 帧第三阶段结束事件
		virtual void OnFrameThirdStageFinished(unsigned int frame_index);

		/// 启动方法
		virtual void Launch();
	};
}