//
// Created by Kasper de Bruin on 23-10-2023.
//
#include "VideoReader.h"

#include "../Utils/Log.h"

Scope<VideoReader> VideoReader::Create(const WindowProps& props)
{
    return CreateScope<VideoReader>(props);
}

VideoReader::VideoReader(const WindowProps& props) {
	CORE_INFO("VideoReader", "VideoReader", "VideoReader created");
    Initialize(props);
}


VideoReader::~VideoReader()
{
	CORE_INFO("VideoReader", "~VideoReader", "VideoReader destroyed");
}

void VideoReader::Terminate(){
	CORE_INFO("VideoReader", "Terminate", "VideoReader terminated");
}

bool VideoReader::Initialize(const WindowProps& props) {
    
    CORE_INFO("VideoReader", "Initialize", "VideoReader initialized");

    try {
        capLeft = cv::VideoCapture(props.Title);
    }
    catch (cv::Exception& e) {
        CORE_ERROR("VideoReader", "Initialize", "VideoReader could not open video file");
    }
    
    return true;
}


bool VideoReader::Read()
{

    frameLeft = cv::Mat();
    capLeft.read(frameLeft);

	if (frameLeft.empty()) {
		//CORE_ERROR("VideoReader", "Read", "VideoReader could not read frame");
		return false;
	}

    cv::resize(frameLeft, frameLeft, cv::Size(640, 480));

    return true;
    
}

bool VideoReader::Show() {
    CORE_INFO("VideoReader", "Show", "VideoReader show frame");
    cv::imshow("Left", frameLeft);
    cv::waitKey(0);
    return true;
}
