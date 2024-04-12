#pragma once

#include "Engine/Core/AbstractChannel.hpp"
#include "Engine/Core/AbstractExecutor.hpp"
#include "Engine/Core/AbstractPort.hpp"
#include "Engine/Core/AbstractProcessor.hpp"
#include "Engine/Core/AbstractWorkflow.hpp"

#include "Framework/Channel.hpp"
#include "Framework/Port.hpp"
#include "Framework/Processor.hpp"
#include "Framework/Workflow.hpp"

#include "Engine/Executors/SerialExecutor.hpp"
#include "Engine/Executors/ParallelExecutor.hpp"

#include "Engine/Processors/EmptyProcessor.hpp"
#include "Engine/Processors/InitializeAction.hpp"
#include "Engine/Processors/LambdaAction.hpp"
#include "Engine/Processors/SwapValueAction.hpp"
#include "Engine/Processors/PassValueAction.hpp"
#include "Engine/Processors/SubmitWorkflowAction.hpp"
#include "Engine/Processors/WaitAction.hpp"
#include "Engine/Processors/AwakeAction.hpp"
#include "Engine/Processors/WaitConditionAction.hpp"
#include "Engine/Processors/NotifyConditionAction.hpp"

#include "Engine/Decorators/DecoratorIf.hpp"

#include "Engine/MacroKeywords.hpp"

#include "Engine/Runtime.hpp"

namespace Galaxy
{

}
