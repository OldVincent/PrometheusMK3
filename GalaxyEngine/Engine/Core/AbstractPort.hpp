#pragma once

#include <string>

namespace Galaxy::Core
{
	/// 流处理器接口
	class AbstractProcessor;
	/// 工作流接口
	class AbstractWorkflow;
	/// 通道接口
	class AbstractChannel;

	namespace Tools
	{
		class PortAccess;
	}

	/**
	 * @brief 抽象端口
	 * @author Vincent
	 * @details
	 *  ~ 端口类提供对通道的自动绑定和数据读写功能。
	 */
	class AbstractPort
	{
		friend class Tools::PortAccess;

	private:
		/// 挂载的通道
		AbstractChannel* MountedChannel {nullptr};
		/// 是否可选
		bool Optional {false};

	public:
		/**
		 * @brief 端口的名称，同时也是要挂载到的通道的名称
		 * @details 注意，在工作流初始化前修改才有效
		 */
		std::string MappingName;

		/**
		 * @brief 获取当前端口是否挂载
		 * @retval true 已挂载
		 * @retval false 未挂载
		 */
		[[nodiscard]] bool IsMounted() const
		{
			return MountedChannel != nullptr;
		}

		/**
		 * @brief 获取当前端口是否为可选端口
		 * @retval true 是可选的，可以不挂载
		 * @retval false 是必要的，必须挂载
		 */
		[[nodiscard]] bool IsOptional() const
		{
			return Optional;
		}
	protected:
		/**
		 * @brief 获取挂载的通道
		 * @return 挂载的通道的指针
		 */
		[[nodiscard]] AbstractChannel* GetMountedChannel() const
		{
			return MountedChannel;
		}

	public:
		/**
		 * @brief 构造函数
		 * @param name 自身名称，将挂载Session上的同名通道
		 * @param host 宿主任务对象指针
		 */
		AbstractPort(std::string name, AbstractProcessor* host, bool optional = false);
	};
}