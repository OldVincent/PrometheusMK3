#pragma once

#include <unordered_map>
#include <tbb/tbb.h>
#include <list>
#include <tuple>
#include <functional>
#include <memory>

#include "../Processors/InitializeAction.hpp"

namespace Galaxy::Core
{
	/// 抽象通道接口
	class AbstractChannel;
	/// 流处理器接口
	class AbstractProcessor;
	/// 抽象执行器类
	class AbstractExecutor;

	namespace Tools
	{
		class WorkflowAccess;
	}

	/**
	 * @brief 抽象工作流类
	 * @author Vincent
	 * @details
	 *  ~ 工作流类提供工作流的管理和流处理单元的自动挂载能力。
	 */
	class AbstractWorkflow
	{
		friend class Tools::WorkflowAccess;
	private:
		/**
		 * @brief 是否已经初始化
		 * @retval true 当流处理器节点的端口都已经挂载到对应通道上
		 * @retval false 当流处理器节点的端口尚未挂载到对应通道上
		 */
		bool Initialized {false};

		/// 通道映射表
		tbb::concurrent_unordered_map<std::string, AbstractChannel*> Channels;
		/// 流处理器列表
		std::list<std::tuple<AbstractProcessor*, AbstractExecutor**>> Processors;
		/// 下一个需要被执行的任务的处理器
		decltype(Processors)::iterator NextProcessor {};

		//==============================
		// 交互操作部分
		//==============================

		/**
		 * @brief 迭代执行
		 * @throw std::runtime_error 当当前执行器为空或流处理器执行器为空
		 * @return 可选，需要将该工作流传递给的执行器，若已达执行链末尾，则返回std::nullopt
		 */
		std::optional<AbstractExecutor*> IterateExecute();

		/**
		 * @brief 初始化任务
		 * @details
		 *  ~ 该任务将在初始化的时候默认被添加到流处理器的头部。
		 */
		BuiltIn::InitializeAction InitializeTask;

		/**
		 * @brief 初始化方法
		 * @note 工作流默认内建初始化处理器，故该方法不需要手动调用，否则OnBegin会重复触发。
		 * @details
		 *  ~ 在挂载端口前，会先调用OnBegin事件。
		 *  ~ 若工作流已标记为已经初始化，则该方法不会重新挂载端口。
		 */
		void Initialize();

	public:
		//==============================
		// 用户事件部分
		// 用户可在此处挂载自定义的事件
		//==============================

		/**
		 * @brief 执行结束事件
		 * @details
		 *  ~ 工作流每次执行结束都会触发该事件。
		 */
		std::function<void()> OnEnd {};
		/**
		 * @brief 执行开始事件
		 * @details
		 *  ~ 工作流每次执行开始都会触发该事件。
		 */
		std::function<void()> OnBegin {};

		/**
		 * @brief 初始化事件
		 * @details 该事件仅在工作流被初始化的时候执行一次。
		 */
		std::function<void()> OnInitialize {};

		/**
		 * @brief 终止化事件
		 * @details 该事件仅在工作流被析构的时候执行一次。
		 */
		std::function<void()> OnFinalize {};

	public:
		/**
		 * @brief 是否要求循环执行
		 * @details 若设置为true，则当工作流抵达末尾时将返回开头继续执行。
		 */
		bool Loop {false};
		/**
		 * @brief 循环执行的停止条件
		 * @details 当该函数器非空，且开启了循环后，达到工作流末尾且函数器返回true时将停止迭代。
		 */
		std::function<bool()> LoopStopCondition {};

		/**
		 * @brief 构造函数
		 * @details
		 *  ~ 将在处理器列表中添加一个内建的初始化处理器，用于调用工作流的初始化方法。
		 */
		AbstractWorkflow();

		/// 析构函数
		~AbstractWorkflow();

		//==============================
		// 操作符重载部分
		//==============================

		/**
		 * @brief 流传出操作符，将提交自身到目标执行器
		 * @param executor 执行器指针
		 * @return 自身
		 */
		AbstractWorkflow& operator>>(AbstractExecutor* executor);

		/**
		 * @brief 流传出操作符，将提交自身到目标执行器
		 * @param executor 执行器引用
		 * @return 自身
		 */
		AbstractWorkflow& operator>>(AbstractExecutor& executor);
	};
}