#include "AbstractExecutor.hpp"
#include "AbstractWorkflow.hpp"
#include "Tools/WorkflowAccess.hpp"
#include <stdexcept>
#include <pthread.h>
#include <thread>

namespace Galaxy::Core
{
	//==============================
	// 线程控制部分
	//==============================

	/**
	 * @brief 设置线程的CPU亲和性
	 * @param pid 线程PID
	 * @param cpus 亲和CPU列表
	 */
	void SetThreadCPUAffinity(std::thread& target, const std::vector<unsigned int>& cpus)
	{
		// 获取线程ID
		pthread_t id = target.native_handle();

		cpu_set_t mask;

		CPU_ZERO(&mask);

		for (const auto index : cpus)
		{
			CPU_SET(index, &mask);
		}
		/// 设置CPU亲和性
		pthread_setaffinity_np(id, sizeof(mask), &mask);
	}

	/// 设置当前线程的线程亲和度
	void SetCurrentThreadCPUAffinity(const std::vector<unsigned int>& cpus)
	{
		cpu_set_t mask;

		CPU_ZERO(&mask);

		for (const auto index : cpus)
		{
			CPU_SET(index, &mask);
		}
		sched_setaffinity(0, sizeof(mask), &mask);
	}

	/// 执行器工作线程
	void ExecutorWorkingThread(AbstractExecutor *executor, const std::vector<unsigned int>& cpus)
	{
		if (!cpus.empty())
		{
			SetCurrentThreadCPUAffinity(cpus);
		}

		executor->LaunchWorkingThread();
	}

	/// 启动工作线程
	void AbstractExecutor::LaunchWorkingThread()
	{
		Working = true;

		while(LifeFlag)
		{
			// 若启用了停止条件，则判断停止条件函数器的返回值
			if (EnableStopCondition && StopCondition)
			{
				if (StopCondition())
				{
					// 满足停止条件则终止循环
					break;
				}
			}
			OnUpdateWorkingThread();
		}

 		Working = false;
	}

	//==============================
	// 线程控制部分
	//==============================

	/// 带有CPU亲和性的构造函数
	AbstractExecutor::AbstractExecutor(std::initializer_list<unsigned int> cpus) : CurrentCPUAffinity(cpus)
	{}

	/// 启动执行器
	void AbstractExecutor::Start()
	{
		if (!Working)
		{
			if (WorkingThread)
			{
				WorkingThread->join();
			}

			// 开启生命开关
			LifeFlag = true;

			WorkingThread = std::make_unique<std::thread>([this]{
				ExecutorWorkingThread(this, this->CurrentCPUAffinity);
			});
		}
	}

	/// 要求执行器停止
	void AbstractExecutor::Stop()
	{
		LifeFlag = false;
	}

	/// 阻塞调用线程
	void AbstractExecutor::Join()
	{
		if (WorkingThread)
		{
			WorkingThread->join();

			WorkingThread.reset();
		}
	}

	/// 设置CPU亲和性
	void AbstractExecutor::SetCPUAffinity(const std::vector<unsigned int> &cpus)
	{
		CurrentCPUAffinity = cpus;
		if (Working)
		{
			SetThreadCPUAffinity(*WorkingThread, cpus);
		}
	}

	/// 设置停止条件
	void AbstractExecutor::SetStopCondition(std::function<bool()> stop_condition)
	{
		EnableStopCondition = true;
		std::unique_lock lock(StopConditionMutex);
		StopCondition = std::move(stop_condition);
	}

	/// 移除停止条件
	void AbstractExecutor::RemoveStopCondition()
	{
		EnableStopCondition = false;
	}

	//==============================
	// 工作流处理部分
	//==============================

	/// 执行工作流
	void AbstractExecutor::InvokeWorkflow(AbstractWorkflow *workflow)
	{
		if (workflow)
		{
			auto result = Tools::WorkflowAccess::IterateExecute(workflow);
			if (result)
			{
				if (!(*result))
				{
					throw std::runtime_error("[AbstractExecutor::InvokeWorkflow] Executor Pointer is Null.");
				}

				(*result)->Submit(workflow);
			}
		}
		else
		{
			throw std::runtime_error("[AbstractExecutor::InvokeWorkflow] Workflow Pointer is Null.");
		}
	}

	/// 提交工作流
	void AbstractExecutor::Submit(AbstractWorkflow *workflow)
	{
		if (workflow)
		{
			Tasks.push(workflow);
		}
		else
		{
			throw std::runtime_error("[AbstractExecutor::Submit] Workflow Pointer is Null.");
		}
	}

}