#include "ContoursDetector.hpp"

namespace RoboPioneers::Prometheus::Processors
{
	/// 执行方法
	void ContoursDetector::Execute()
	{
		auto& binary_picture = *BinaryPicture;
		auto& contours = *Contours;
		contours.clear();

		cv::findContours(binary_picture, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	}
}