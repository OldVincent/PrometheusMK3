#include "PictureCutter.hpp"

#include "../../Modules/ImageDebugUtility.hpp"

namespace RoboPioneers::Prometheus::Processors
{
	/// 执行方法
	void PictureCutter::Execute()
	{
		auto& area = *CuttingArea;
		auto& picture = *CuttingPicture;

		if (!area.empty() && area.area() > 0)
		{
			if (area.x < 0) area.x = 0;
			if (area.y < 0) area.y = 0;
			if (area.x + area.width > picture.cols) area.width = picture.cols - area.x;
			if (area.y + area.height > picture.rows) area.height = picture.rows - area.y;

			picture = picture(area);
		}
	}
}
