#pragma once

#include <string>
#include <initializer_list>
#include <optional>

namespace Galaxy::Core
{
	class AbstractWorkflow;
	class AbstractProcessor;
	class AbstractExecutor;
	class AbstractChannel;

	namespace Tools
	{
		/**
		 * @brief 工作流访问器
		 * @author Vincent
		 * @details
		 *  ~ 该工具类用于在不增加用户心智负担的情况下减少友元访问声明。
		 */
		class WorkflowAccess
		{
		public:
			/// 注册流处理器
			static void RegisterProcessor(AbstractWorkflow* workflow,
								 AbstractProcessor* processor, AbstractExecutor** executor);
			/// 注册通道
			static void RegisterChannel(AbstractWorkflow* workflow,
							   AbstractChannel* channel, const std::string& name);
			/// 注册通道
			static void RegisterChannel(AbstractWorkflow* workflow,
							   AbstractChannel* channel, const std::initializer_list<const char*>& names);
			/// 迭代执行
			static auto IterateExecute(AbstractWorkflow* workflow) -> std::optional<AbstractExecutor*>;
			/// 初始化
			static void Initialize(AbstractWorkflow* workflow);

			/// 获取通道
			static AbstractChannel* GetChannel(AbstractWorkflow* workflow, const std::string& name);

			/// 获取当前的执行器
			static AbstractExecutor* GetCurrentExecutor(AbstractWorkflow* workflow);
		};
	}

}