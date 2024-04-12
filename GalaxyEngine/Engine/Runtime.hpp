#pragma once

#include <unordered_set>
#include <shared_mutex>
#include "Executors/RealtimeExecutor.hpp"
#include "Executors/WorkflowWaitingExecutor.hpp"
#include "Executors/WorkflowDeleterExecutor.hpp"

namespace Galaxy
{
	namespace Core
	{
		class AbstractExecutor;
	}
	/**
	 * @brief 运行时对象
	 * @details
	 *  ~ 运行时
	 */
	class Runtime
	{
	public:
		//==============================
		// 单实例控制部分
		//==============================
		/**
		 * @brief 获取运行时实例
		 * @return 运行时实例
		 */
		static Runtime* GetInstance();

	protected:
		/**
		 * @brief 托管的执行器列表，并且该列表中都是尚未通过运行时等待或者停止的执行器
		 * @details
		 *  ~ 执行器经由运行时的Stop或Join方法结束，将会被从该集合中移除。
		 */
		std::unordered_set<Core::AbstractExecutor*> ManagedExecutors;
		/// 托管执行器集合互斥量
		std::shared_mutex ManagedExecutorsMutex;

	public:

		/**
		 * @brief 当前执行器
		 * @details
		 *  ~ 其本质为实时执行器，但实时执行器总是在调用线程中工作，所以总是在“当前”线程中工作。
		 */
		RealtimeExecutor CurrentExecutor;

		/**
		 * @brief 工作区删除器
		 */
		WorkflowDeleterExecutor WorkflowDeleter;

		/**
		 * @brief 工作区等待区
		 *
		 */
		WorkflowWaitingExecutor WorkflowWaitingZone;

		/**
		 * @brief 当前执行器指针
		 * @details 提供该项是为了便于流处理器绑定。
		 */
		Core::AbstractExecutor* CurrentExecutorPointer {&CurrentExecutor};

		/**
		 * @brief 停止所有的执行器
		 * @details
		 *  ~ 该方法将停止所有已经托管的执行器。
		 */
		void StopAllExecutors();
		/**
		 * @brief 注册允许接受通知的执行器
		 * @param executors 执行器列表
		 * @details
		 *  ~ 注册完成后，StopAllExecutors方法将可以影响到该执行器。
		 */
		void RegisterExecutors(std::initializer_list<Core::AbstractExecutor*> executors);
		/**
		 * @brief 注销允许接受通知的执行器
		 * @param executors 执行器列表
		 * @details
		 *  ~ 注销完成后，StopAllExecutors方法将不再可以影响到该执行器。
		 */
		void UnregisterExecutors(std::initializer_list<Core::AbstractExecutor*> executors);
	};
}