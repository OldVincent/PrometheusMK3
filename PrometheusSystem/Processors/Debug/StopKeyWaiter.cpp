#include "StopKeyWaiter.hpp"

#include <opencv4/opencv2/opencv.hpp>

namespace RoboPioneers::Prometheus::Processors::DebugPackage
{
	/// 执行方法
	void StopKeyWaiter::Execute()
	{
		if (cv::waitKey(1) == StopKey)
		{
			GetWorkflow()->Loop = false;
			cv::destroyAllWindows();
			Galaxy::Runtime::GetInstance()->StopAllExecutors();
		}
	}
}