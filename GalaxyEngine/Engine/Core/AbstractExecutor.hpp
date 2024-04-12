#pragma once

#include <atomic>
#include <thread>
#include <memory>
#include <vector>
#include <tbb/tbb.h>
#include <initializer_list>
#include <shared_mutex>

namespace Galaxy::Core
{
	/// 抽象管道类
	class AbstractWorkflow;

	/**
	 * @brief 执行器类
	 * @author Vincent
	 * @details
	 *  ~ 执行器类用于在额外的线程中执行任务。
	 *  ~ 一个应用程序中执行器的数量应当是有限的。
	 *  ~ 执行器可以设置CPU亲和性，在多数系统下，CPU亲和性即线程绑定在某个特定CPU上工作的能力。
	 */
	class AbstractExecutor
	{
		/// 声明执行器工作线程函数为友元
		friend void ExecutorWorkingThread(AbstractExecutor* executor, const std::vector<unsigned int>& cpus);

	private:
		/// 线程是否正在工作
		std::atomic_bool Working {false};
		/// 工作线程生命旗标
		std::atomic_bool LifeFlag {false};

		/// 工作线程
		std::unique_ptr<std::thread> WorkingThread;

		/// 启动工作线程
		void LaunchWorkingThread();

		/// 当前CPU亲和性
		std::vector<unsigned int> CurrentCPUAffinity {};

		/// 终止条件互斥量
		std::shared_mutex StopConditionMutex;
		/// 终止条件
		std::function<bool()> StopCondition;
		/// 是否启用终止条件
		std::atomic_bool EnableStopCondition {false};

	protected:
		/// 工作线程生命循环更新事件
		virtual void OnUpdateWorkingThread() = 0;

		/// 被委派的任务队列
		tbb::concurrent_queue<AbstractWorkflow*> Tasks;

		/**
		 * @brief 执行工作流
		 * @param workflow 工作流指针
		 * @throw std::runtime_error 当工作流指针为空
		 * @details
		 *  ~ 该方法是给派生类提供的，用于与抽象通道交互。
		 *  ~ 方法内部将调用工作流暴露的接口，阻塞式执行完毕后，将该管道提交给下一个执行器。
		 */
		static void InvokeWorkflow(AbstractWorkflow* workflow);

	public:
		//==============================
		// 构造与析构部分
		//==============================

		/// 默认构造函数
		AbstractExecutor() = default;

		/**
		 * @brief 设置CPU亲和性的构造函数
		 * @param cpus 处理器编号列表
		 */
		AbstractExecutor(std::initializer_list<unsigned int> cpus);

		//==============================
		// 线程控制部分
		//==============================

		/**
		 * @brief 启动执行器
		 * @details
		 *  ~ 若执行器未启动，则该方法将启动执行器。
		 *  ~ 若执行器已经处于工作状态，则该方法不会进行操作。
		 */
		virtual void Start();
		/**
		 * @brief 停止执行器
		 * @details
		 *  ~ 该方法将要求执行器停止，发出命令后立即返回，但不保证返回时执行器已经结束。
		 */
		virtual void Stop();

		/**
		 * @brief 阻塞调用线程
		 * @details
		 *  ~ 该方法将阻塞调用线程，直至执行器结束。
		 */
		virtual void Join();

		/**
		 * @brief 获取当前线程是否正在工作
		 * @retval true 当工作线程正处于工作状态
		 * @retval false 当工作线程尚未处于工作状态
		 */
		[[nodiscard]] virtual bool IsWorking() const
		{
			return Working;
		}

		//==============================
		// 线程属性部分
		//==============================

		/**
		 * @brief 设置终止条件
		 * @param stop_condition 终止条件
		 * @details
		 *  ~ 将被设置为生命旗标和该函数器返回值为false。
		 *  ~ 当LifeFlag为false或终止条件函数器返回true时，线程将自动结束。
		 */
		void SetStopCondition(std::function<bool()> stop_condition);

		/**
		 * @brief 移出终止条件
		 * @details
		 *  ~ 终止条件将被移除，线程的生命周期将只受生命旗标影响。
		 */
		void RemoveStopCondition();

		/**
		 * @brief 设置CPU亲和性
		 * @param cpus 处理器编号列表
		 */
		void SetCPUAffinity(const std::vector<unsigned int>& cpus);

		//==============================
		// 工作流交互部分
		//==============================

		/**
		 * @brief 提交工作流
		 * @param workflow 将要被执行的工作流
		 * @throw std::runtime_error 当工作流指针为空指针
		 */
		virtual void Submit(AbstractWorkflow* workflow);

		/**
		 * @brief 查询任务队列是否为空
		 * @retval true 当任务队列为空
		 * @retval false 当任务队列非空
		 */
		[[nodiscard]] virtual bool IsEmpty() const
		{
			return Tasks.empty();
		}

		//==============================
		// 操作符部分
		//==============================

		/**
		 * @brief 流传入操作符，将提交工作流到自身
		 * @param workflow 需要被提交的工作流指针
		 * @return 自身的应用
		 */
		virtual AbstractExecutor& operator<<(AbstractWorkflow* workflow)
		{
			Submit(workflow);
			return *this;
		}

		/**
		 * @brief 流传入操作符，将提交工作流到自身
		 * @param workflow 需要被提交的工作流引用
		 * @return 自身的应用
		 */
		virtual AbstractExecutor& operator<<(AbstractWorkflow& workflow)
		{
			Submit(&workflow);
			return *this;
		}
	};
}