//
// Created by Kasper de Bruin on 22-10-2023.
//

#include "ExampleLayer.h"
#include <chrono>

ExampleLayer::ExampleLayer()
: Layer("Example"), videoReaderLeft(nullptr), videoReaderRight(nullptr)
{
    CL_WARN("ExampleLayer::ExampleLayer constructor");
}

void ExampleLayer::OnAttach() {
    CL_WARN("ExampleLayer::OnAttach");

    videoReaderLeft = VideoReader::Create(
        VideoProps(
            "D:\\OPENCV\\Assets\\Left.mp4",
            4000,
            3000,
            25
        )
    );    

    videoReaderRight = VideoReader::Create(
        VideoProps(
            "D:\\OPENCV\\Assets\\Right.mp4",
            4000,
            3000,
            25
        )
    );

    cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 3000, 0, 4000 / 2, 0, 3000, 3000 / 2, 0, 0, 1);
    
    videoProcessor = VideoProcessor::Create(
        VideoProcessorPropsFinal(),
        VideoProcessorProps(
            std::array <float, 3>  { -0.199f, -0.1300f, -0.0150f },
            cameraMatrix
        ),
        VideoProcessorProps(
            std::array <float, 3> { -0.4190f, 0.0780f, -0.0460f },
            cameraMatrix
        )
    );

}

void ExampleLayer::OnUpdate() {
    
    CL_TRACE("ExampleLayer::OnUpdate");

    auto start = std::chrono::high_resolution_clock::now();  // Store the start time

    videoReaderLeft->Read();
    videoReaderRight->Read();
    
    videoProcessor->ProcessFrames(
		videoReaderLeft->GetFrame(),
		videoReaderRight->GetFrame()
	);

    cv::Mat frame;

    videoProcessor->GetFinalFrame().download(frame);
    cv::resize(frame, frame, cv::Size(1280, 720));
    cv::imshow("Right", frame);

    auto end = std::chrono::high_resolution_clock::now();  // Store the end time
    std::chrono::duration<double, std::milli> processing_time = end - start;  // Calculate the processing time

    int frame_time = 1000 / videoProcessor->GetFps();  // Time for each frame in milliseconds
    int delay = frame_time - processing_time.count();  // Adjust the delay based on the processing time

    if (delay > 0)
    {
        cv::waitKey(delay);
    }
    else
    {
        cv::waitKey(1);  // Minimum wait time to process user input
    }
	
}

void ExampleLayer::OnDetach() {
    CL_TRACE("ExampleLayer::OnDetach");
}

