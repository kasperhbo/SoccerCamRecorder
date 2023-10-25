//
// Created by Kasper de Bruin on 22-10-2023.
//

#include "ExampleLayer.h"
#include "SoccerVisualization/Utils/TimeUtils.h"
#include "SoccerVisualization/VideoProcessing/Soc_VideoProcessor.h"
#include <chrono>

ExampleLayer::ExampleLayer()
: Layer("Example")
{
    CL_WARN("ExampleLayer::ExampleLayer constructor");
}

void ExampleLayer::OnAttach() {
    CL_WARN("ExampleLayer::OnAttach");
	bool saveVideo = true;
	bool playingWithGui = true;

	const std::string clipSaveNameLocation = "D:\\opencv\\assets\\saves\\";
	const std::string clipSaveName = clipSaveNameLocation + TimeUtils::GetFormattedString() + "final.h264";
	const std::string clipLeftName = "D:\\opencv\\assets\\Recordings\\Left_0009.mp4";
	const std::string clipRightName = "D:\\OPENCV\\Assets\\Recordings\\Right_0009.mp4";
	
	Soc_VideoProcessor* processor = new Soc_VideoProcessor(clipLeftName, clipRightName, clipSaveName, true, false, 25.0, 4000, 1500);
	processor->Initialize();

	float delay = 1000 / 25.0;

	while (true) {
		//clock_t startTime = clock();

		if (!processor->ProcessFrame())
		{
			processor->Terminate();
			Application::Get().Close();
			break;
		}

		/*while (clock() - startTime < delay) {
			int key = cv::waitKey(1);
		}*/
	}
}

void ExampleLayer::OnUpdate() 
{    
    CL_TRACE("ExampleLayer::OnUpdate");	
}

void ExampleLayer::OnDetach() {
    CL_TRACE("ExampleLayer::OnDetach");
}

