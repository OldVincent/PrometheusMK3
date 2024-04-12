#include "Runtime.hpp"

namespace Galaxy
{
	/// 获取运行时实例
	Runtime *Runtime::GetInstance()
	{
		static Runtime RuntimeInstance;

		return &RuntimeInstance;
	}

	/// 停止所有的执行器
	void Runtime::StopAllExecutors()
	{
		std::unique_lock lock(ManagedExecutorsMutex);
		for (auto* executor : ManagedExecutors)
		{
			executor->Stop();
		}
	}

	/// 注册执行器
	void Runtime::RegisterExecutors(std::initializer_list<Core::AbstractExecutor*> executors)
	{
		std::unique_lock lock(ManagedExecutorsMutex);
		for (auto* executor : executors)
		{
			ManagedExecutors.insert(executor);
		}
	}

	/// 注销执行器
	void Runtime::UnregisterExecutors(std::initializer_list<Core::AbstractExecutor *> executors)
	{
		std::unique_lock lock(ManagedExecutorsMutex);
		for (auto* executor : executors)
		{
			ManagedExecutors.erase(executor);
		}
	}
}
