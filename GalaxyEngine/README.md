# Galaxy Engine

## 简介

"银河"流处理引擎。该引擎提供执行器、工作流、流处理器、通道和端口五大元素。
执行器可以绑定一个或数个指定的CPU，并在之上并行或串行地执行工作流。
工作流由通道和流处理器组成。
流处理器通过端口来绑定通道、读写通道值，并最终实现与其他流处理器共享数据。
声明流处理器的时候需要表明其在哪个执行器上工作，从而确定其在哪个CPU上工作。

对于工作流而言，其内的流处理器总是从上到下串行地逐个执行，并且可以指定流处理器在哪个执行器上工作。
执行器往往与CPU绑定，从而实质上可以指定流处理器在哪个CPU上运行。

流处理器通过端口来访问通道值。
工作流首次被执行时，流处理器的端口会自动挂载到工作流声明中同名的通道。
考虑到流处理器端口名称可能各异，但同时又有共享数据的需求，所以通道可以同时具有多个名称。

## 注意

不同流处理器间可能会发生执行器的切换，即使不切换，也会在同一执行器上重新排队，
极大可能导致CPU缓存失效；故若每个流处理器任务量太少，则有可能无法体现出流处理结构的优势。

## 示例

```c++
using namespace Galaxy;

class SampleProcessor AsProcessor
{
Requirement:
    Require(int, IntChannel);
    Require(int, boolValue);

	Process
	{
		std::cout << IntChannel.Get() << std::endl;
		EXPECT_TRUE(boolValue.Acquire());
		IntChannel.Acquire()++;
		std::cout<< "Thread on CPU " << sched_getcpu() << std::endl;
	}
};

class SampleWorkFlow AsWorkflow
{
Executors:
    NeedExecutor(BigCore);

Channels:
    Channel<int> IntChannel MappingName(("Int1", "Int2", "IntChannel"), 6);
    Channel<bool> BoolChannel MappingName(("bool", "boolValue"), true);
    Channel<long> LongChannel MappingName("Long");

Procedure:
    SampleProcessor SampleProcessorInstance1 On(BigCore);
    SampleProcessor SampleProcessorInstance2 On(BigCore);
};

int main()
{
    ParallelExecutor engine;

    SampleWorkFlow sample_flow1{.BigCore = &engine};
    SampleWorkFlow sample_flow2{.BigCore = &engine};

    engine.Start();
    engine << sample_flow1 << sample_flow1;
    sample_flow2 >> engine;
    engine.SetStopCondition([instance = &engine](){return instance->IsEmpty();});
    engine.Join();
}
```