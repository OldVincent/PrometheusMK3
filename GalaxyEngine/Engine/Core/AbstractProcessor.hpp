#pragma once

#include <tbb/tbb.h>
#include <string>

namespace Galaxy::Core
{
	/// 抽象端口类
	class AbstractPort;
	/// 抽象工作流类
	class AbstractWorkflow;
	/// 抽象执行器类
	class AbstractExecutor;

	namespace Tools
	{
		class ProcessorAccess;
	}

	/**
	 * @brief 抽象流处理节点
	 * @author Vincent
	 * @details
	 *  ~ 抽象任务类提供了对端口的自动管理能力。
	 */
	class AbstractProcessor
	{
		/// 允许流处理器访问器访问该类
		friend class Tools::ProcessorAccess;

	private:
		/// 端口映射表
		tbb::concurrent_unordered_set<AbstractPort*> Ports;

		/// 存放目标执行器指针的指针
		AbstractExecutor** TargetExecutor {nullptr};
		/// 宿主工作流
		AbstractWorkflow* HostWorkflow {nullptr};

	protected:
		/**
		 * @brief 阻塞工作流旗标
		 * @details
		 *  ~ 若此项为true，则工作流执行完毕该项后的迭代将中断，且结束事件不会被调用。
		 *  ~ 起效后，该项将被重置为false。
		 */
		bool PauseWorkflowFlag {false};

		/**
		 * @brief 停止工作流旗标
		 * @details
		 *  ~ 若此项为true，则工作流执行完毕该项后迭代将停止，且结束事件会被调用。
		 *  ~ 起效后，该项将被重置为false。
		 */
		bool StopWorkflowFlag {false};

		/**
		 * @brief 获取目标执行器
		 * @return 指向目标执行器的指针的指针，指向工作流中用于绑定执行器的指针的指针
		 * @pre 对象被公开的指定工作流和执行器的构造函数构造。
		 */
		[[nodiscard]] inline AbstractExecutor** GetExecutor() const
		{
			return TargetExecutor;
		}

		/**
		 * @brief 获取宿主工作流
		 * @return 宿主工作流的指针
		 * @pre 对象被公开的指定工作流和执行器的构造函数构造。
		 */
		[[nodiscard]] inline AbstractWorkflow* GetWorkflow() const
		{
			return HostWorkflow;
		}

		/**
		 * @brief 默认构造函数
		 * @details
		 *  ~ 该方法是提供给不遵循寻常工作流程的流处理器类执行的。
		 */
		AbstractProcessor() = default;

		/**
		 * @brief 执行方法
		 * @details
		 *  ~ 该方法将在执行器的线程中执行。
		 *  ~ 该方法是提供给派生流处理器类实现的。
		 */
		virtual void Execute() = 0;

		/**
		 * @brief 初始化事件
		 * @details 当工作流被初始化的时候调用
		 */
		virtual void OnInitialize() {};
		/**
		 * @brief 终止化事件
		 * @details 当工作流被终止化的时候调用
		 */
		virtual void OnFinalize() {};

	public:
		/**
		 * @brief 构造函数
		 * @param target_executor 目标执行器，指名该流处理器发送到何处执行
		 * @param host 宿主管道
		 */
		AbstractProcessor(AbstractExecutor** target_executor, AbstractWorkflow* host);
	};
}