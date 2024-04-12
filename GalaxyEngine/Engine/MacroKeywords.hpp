#pragma once

#ifndef GALAXY_MACRO_KEYWORDS
#define GALAXY_MACRO_KEYWORDS

#include <memory>

//==============================
// 通道关键词
//==============================

#ifndef Provide
/**
 * @brief 通道提供关键字
 * @param first （可选）初始化值
 * @param other 为字符串，为名称列表
 */
#define Provide(...) {this, __VA_ARGS__}
#endif

//==============================
// 端口关键词
//==============================

#ifndef Require
/**
 * @brief 声明需求通道
 * @param Type 需求类型
 * @param Name 通道名称
 */
# define Require(Type, Symbol) Galaxy::Port<Type> Symbol = {#Symbol, this}
#endif

#ifndef RequireOptional
/**
 * @brief 声明可选的需求通道
 * @param Type 需求类型
 * @param Name 通道名称
 */
# define RequireOptional(Type, Symbol) Galaxy::Port<Type> Symbol = {#Symbol, this, true}
#endif


//==============================
// 流处理器关键词
//==============================

#ifndef On
/**
 * @brief 指定执行器来执行该流处理器，并可选设定需要的构造参数
 */
#define On(ExecutorReference, ...) {&ExecutorReference, this, ##__VA_ARGS__}
#endif

#ifndef Requirement
/**
 * @brief 需求描述部分开始标签
 * @details 该标签表明后续的部分为需求描述部分
 */
#define Requirement public: using Galaxy::Processor::Processor; public
#endif

#ifndef NoRequirement
/**
 * @brief 需求描述部分开始标签
 * @details 该标签表明该流处理器不需要通道
 */
#define NoRequirement public: using Galaxy::Processor::Processor
#endif

#ifndef AsProcessor
/**
 * @brief 流处理器声明后缀
 * @details 该后缀表明该类为流处理器
 */
#define AsProcessor : public Galaxy::Processor
#endif

#ifndef Process
#define Process void Execute() override
#endif

#ifndef Configure
/**
 * @brief 配置关键字
 * @details
 *  ~ 该关键词表明该流处理器允许配置。
 */
#define Configure(ClassName, ...) \
template<typename ExecutorType, typename WorkflowType, \
	typename = typename std::enable_if<std::is_base_of_v<Galaxy::Core::AbstractExecutor, ExecutorType>>, \
	typename = typename std::enable_if<std::is_base_of_v<Galaxy::Core::AbstractWorkflow, WorkflowType>>> \
ClassName(ExecutorType** executor, WorkflowType* workflow, ##__VA_ARGS__) : \
Galaxy::Processor((Galaxy::Core::AbstractExecutor**)(executor), (Galaxy::Core::AbstractWorkflow*)(workflow))
#endif

#ifndef Name
/**
 * @brief 名称配置选项
 * @details 该关键词表明配置行中可以提供该端口的名称
 */
#define Name(PortSymbol) std::string name_for_##PortSymbol
#endif

#ifndef OptionalName
/**
 * @brief 可选名称配置项
 * @details 该关键词表明配置行中可以提供该端口的名称
 */
#define OptionalName(PortSymbol) std::string name_for_##PortSymbol = #PortSymbol
#endif

#ifndef ApplyName
/// 将配置行中的名称应用到端口
#define ApplyName(PortSymbol) PortSymbol.MappingName = std::move(name_for_##PortSymbol);
#endif

//==============================
// 执行器关键词
//==============================

#ifndef NeedExecutor
/**
 * @brief 需要任意类型的执行器
 * @param Name 执行器在工作流中的引用名称o
 */
#define NeedExecutor(Name) Galaxy::SerialExecutor* Name
#endif

#ifndef NeedSerialExecutor
/**
 * @brief 需要串行执行器
 * @param Name 执行器在工作流中的引用名称
 */
#define NeedSerialExecutor(Name) Galaxy::SerialExecutor* Name
#endif

#ifndef NeedParallelExecutor
/**
 * @brief 需要并行执行器
 * @param Name 执行器在工作流中的引用名称
 */
#define NeedParallelExecutor(Name) Galaxy::ParallelExecutor* Name
#endif

#ifndef Channels
/**
 * @brief 通道描述部分开始标签
 * @details
 *  ~ 该标签表明后续的部分为持有的通道的描述。
 */
#define Channels public
#endif

#ifndef Procedure
/**
 * @brief 处理流程描述部分开始标签
 * @details
 *  ~ 该标签表明后续的部分为处理流程的描述。
 *  ~ 处理流程将按描述顺序从上向下执行。
 */
#define Procedure public
#endif

//==============================
// 工作区关键字
//==============================

#ifndef Executors
/**
 * @brief 引入的执行器描述部分开始标签
 * @details
 *  ~ 该标签表明后续的部分为需要引入的执行器的描述。
 */
#define Executors public
#endif

#ifndef AsWorkflow
/**
 * @brief 工作流声明后缀
 * @details 该后缀表明该类为工作流
 */
#define AsWorkflow : public Galaxy::Workflow
#endif

#endif //GALAXY_MACRO_KEYWORDS