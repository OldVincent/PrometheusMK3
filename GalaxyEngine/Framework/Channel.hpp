#pragma once

#include <initializer_list>
#include <type_traits>
#include <string>
#include <memory>
#include "GalaxyEngine/Engine/Core/AbstractChannel.hpp"

namespace Galaxy
{
	/**
	 * @brief 通道模板类
	 * @tparam ValueType 值类型
	 * @author Vincent
	 * @details
	 *  ~ 通道使用共享智能指针来管理内存，从而可以支持通道建立上下游连接，
	 *    并且当部分通道析构时，其余通道依然可以正常工作。
	 */
	template<typename ValueType>
	class Channel : public Core::AbstractChannel
	{
	protected:
		/**
		 * @brief 值的共享指针
		 * @details
		 *  ~ 使用共享智能指针来保存的值得内存，从而使得上游通道析构之后，下游通道依然可以使用其内的值，从而得以建立通道连接机制。
		 */
		std::shared_ptr<ValueType> ValuePointer {nullptr};

	public:
		//==============================
		// 构造与析构部分
		//==============================

		/// 多名称带初值构造函数
		template<typename WorkflowType, typename... ArgumentsType>
		Channel(WorkflowType* host, ValueType initial_value, ArgumentsType... arguments):
			AbstractChannel((Core::AbstractWorkflow*)(host), {arguments...})
		{
				ValuePointer = std::make_shared<ValueType>(initial_value);
		}

		/// 多名称无初值构造函数
		template<typename WorkflowType, typename... ArgumentsType>
		explicit Channel(WorkflowType *host, ArgumentsType... arguments):
				AbstractChannel((Core::AbstractWorkflow*)(host), {arguments...})
		{
			ValuePointer = std::make_shared<ValueType>();
		}

		/**
		 * @brief 单名称的构造函数
		 * @param name 名称
		 * @param host 宿主
		 * @param initial_value 初始值
		 */
		Channel(Core::AbstractWorkflow *host, const char* name, ValueType initial_value) :
				AbstractChannel(host, name)
		{
			ValuePointer = std::make_shared<ValueType>(initial_value);
		}

		/// 无初值构造函数
		Channel(Core::AbstractWorkflow *host, const char* name) :
			AbstractChannel(host, name)
		{
			ValuePointer = std::make_shared<ValueType>();
		}

		/// 禁止拷贝构造
		Channel(const Channel<ValueType>& target) = delete;
		/// 禁止移动构造
		Channel(Channel<ValueType>&& target) = delete;

		//==============================
		// 基本访问部分
		//==============================

		/// 取值操作符，返回值的引用
		inline ValueType& operator*()
		{
			return *ValuePointer;
		}

		/// 指针访问操作符，适用于值为指针的情况
		inline ValueType operator->()
		{
			return ValuePointer;
		}

		/**
		 * @brief 获取值
		 * @return 值的引用
		 */
		inline ValueType& Acquire()
		{
			return *ValuePointer;
		}

		/**
		 * @brief 使当前通道成为目标通道的下游通道，即连接在一起
		 * @param upstream_channel 目标上游通道
		 */
		void Connect(const Channel<ValueType>& upstream_channel)
		{
			ValuePointer = upstream_channel.ValuePointer;
		}

		//==============================
		// 其他操作符部分
		//==============================

		/**
		 * @brief 显式转换操作符
		 * @return 值的引用
		 */
		explicit operator ValueType&()
		{
			return *ValuePointer;
		}

		//==============================
		// 赋值操作符部分
		//==============================

		/// 拷贝赋值操作符
		template<typename = typename std::enable_if<std::is_copy_assignable_v<ValueType>>>
		Channel<ValueType>& operator=(const ValueType& value) noexcept
		{
			Acquire() = value;
			return *this;
		}

		/// 拷贝赋值操作符
		template<typename = typename std::enable_if<std::is_copy_assignable_v<ValueType>>>
		Channel<ValueType>& operator=(const Channel<ValueType>& target) noexcept
		{
			if (&target != this)
			{
				Acquire() = target.ValuePointer;
			}
			return *this;
		}

		/// 移动赋值操作符
		template<typename = typename std::enable_if<std::is_move_assignable_v<ValueType>>>
		Channel<ValueType>& operator=(ValueType&& value) noexcept
		{
			Acquire() = std::move(value);
			return *this;
		}

		/// 移动赋值操作符
		template<typename = typename std::enable_if<std::is_move_assignable_v<ValueType>>>
		Channel<ValueType>& operator=(Channel<ValueType>&& target) noexcept
		{
			Acquire() = std::move(target.ValuePointer);
			return *this;
		}

		//==============================
		// 流传输操作符部分
		//==============================

		/// 移动流传输操作符
		template<typename = typename std::enable_if<std::is_move_assignable_v<ValueType>>>
		Channel<ValueType>& operator<<(Channel<ValueType>&& target) noexcept
		{
			Acquire() = std::move(target.ValuePointer);
			return *this;
		}

		/// 拷贝流传输操作符
		template<typename = typename std::enable_if<std::is_copy_assignable_v<ValueType>>>
		Channel<ValueType>& operator<<(const Channel<ValueType>& target) noexcept
		{
			if (&target != this)
			{
				Acquire() = target.ValuePointer;
			}
			return *this;
		}

		/// 移动流传输操作符
		template<typename = typename std::enable_if<std::is_move_assignable_v<ValueType>>>
		Channel<ValueType>& operator<<(ValueType&& value) noexcept
		{
			Acquire() = std::move(value);
			return *this;
		}

		/// 拷贝流传输操作符
		template<typename = typename std::enable_if<std::is_copy_assignable_v<ValueType>>>
		Channel<ValueType>& operator<<(const ValueType& value) noexcept
		{
			Acquire() = value;
			return *this;
		}

		/// 流传出操作符
		template<typename = typename std::enable_if<std::is_copy_assignable_v<ValueType>>>
		Channel<ValueType>& operator>>(ValueType& value) noexcept
		{
			value = Acquire();
			return *this;
		}

		/// 流传出操作符
		template<typename = typename std::enable_if<std::is_move_assignable_v<ValueType>>>
		Channel<ValueType>& operator<<(Channel<ValueType>& target) noexcept
		{
			target.Acquire() = Acquire();
			return *this;
		}
	};
}