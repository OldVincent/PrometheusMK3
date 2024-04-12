#pragma once

#include "../Engine/Core/AbstractPort.hpp"

#include <boost/preprocessor.hpp>

#include "Channel.hpp"

namespace Galaxy
{
	/**
	 * @brief 端口模板类
	 * @tparam ValueType 值类型
	 * @author Vincent
	 * @details
	 *  ~ 端口类用于读写指定的通道。
	 */
	template<typename ValueType>
	class Port : public Core::AbstractPort
	{
	public:
		using AbstractPort::AbstractPort;

		//==============================
		// 数据控制部分
		//==============================

		/**
		 * @brief 获取值
		 * @return 通道中的值的引用
		 */
		inline ValueType& Acquire()
		{
			return static_cast<Channel<ValueType>*>(this->GetMountedChannel())->Acquire();
		}

		/**
		 * @brief 设置值
		 * @param value 值
		 */
		void Set(const ValueType& value)
		{
			Acquire() = value;
		}

		/**
		 * @brief 用移动的方式设置值
		 * @param value 值的右值引用
		 */
		void Set(ValueType&& value)
		{
			Acquire() = std::move(value);
		}

		/**
		 * @brief 获取值
		 * @return 值的常值引用
		 */
		const ValueType& Get()
		{
			return Acquire();
		}

		//==============================
		// 操作符重载部分
		//==============================

		/**
		 * @brief 取值操作符
		 * @return 值的引用
		 */
		ValueType& operator*()
		{
			return static_cast<Channel<ValueType>*>(this->GetMountedChannel())->Acquire();
		}

		/**
		 * @brief 指针访问操作符
		 * @return 适用于值的类型为指针时的情形
		 */
		ValueType operator->()
		{
			return static_cast<Channel<ValueType>*>(this->GetMountedChannel())->Acquire();
		}

		//==============================
		// 赋值操作符部分
		//==============================

		/**
		 * @brief 设置值
		 * @param value 值
		 * @return 自身
		 */
		template<typename = typename std::enable_if<std::is_copy_assignable_v<ValueType>>>
		Port<ValueType>& operator=(const ValueType& value)
		{
			Set(value);
			return *this;
		}

		/**
		 * @brief 设置值
		 * @param value 值
		 * @return 自身
		 */
		template<typename = typename std::enable_if<std::is_move_assignable_v<ValueType>>>
		Port<ValueType>& operator=(ValueType&& value)
		{
			Set(value);
			return *this;
		}

		/// 拷贝赋值操作符
		template<typename = typename std::enable_if<std::is_copy_assignable_v<ValueType>>>
		Port<ValueType>& operator=(const Port<ValueType>& target) noexcept
		{
			if (&target != this)
			{
				Set(target.Get());
			}
			return *this;
		}

		//==============================
		// 流传输操作符部分
		//==============================

		/// 拷贝流传输操作符
		template<typename = typename std::enable_if<std::is_copy_assignable_v<ValueType>>>
		Port<ValueType>& operator<<(const Port<ValueType>& target) noexcept
		{
			if (&target != this)
			{
				Set(target.Value);
			}
			return *this;
		}

		/// 移动流传输操作符
		template<typename = typename std::enable_if<std::is_move_assignable_v<ValueType>>>
		Port<ValueType>& operator<<(ValueType&& value) noexcept
		{
			Set(std::move(value));
			return *this;
		}

		/// 拷贝流传输操作符
		template<typename = typename std::enable_if<std::is_copy_assignable_v<ValueType>>>
		Port<ValueType>& operator<<(const ValueType& value) noexcept
		{
			Set(value);
			return *this;
		}

		/// 流传出操作符
		template<typename = typename std::enable_if<std::is_copy_assignable_v<ValueType>>>
		Port<ValueType>&& operator>>(ValueType& value) noexcept
		{
			value = Get();
			return *this;
		}

		/// 流传出操作符
		template<typename = typename std::enable_if<std::is_copy_assignable_v<ValueType>>>
		Port<ValueType>& operator>>(Port<ValueType>& target) noexcept
		{
			if (&target != this)
			{
				target.Set(Get());
			}
			return *this;
		}
	};
}