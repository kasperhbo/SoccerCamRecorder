//
// Created by Kasper de Bruin on 23-10-2023.
//
#include "VideoReader.h"

#include "SoccerVisualization/Utils/VideoUtils.h"

Scope<VideoReader> VideoReader::Create(const VideoProps& props)
{
    return CreateScope<VideoReader>(props);
}

VideoReader::VideoReader(const VideoProps& props) { 
    Initialize(props);
}


VideoReader::~VideoReader()
{
    //delete frame;
}

void VideoReader::Terminate(){

}

bool VideoReader::Initialize(const VideoProps& props) {
    m_videoData.Width = props.Width;
    m_videoData.Height = props.Height;
    m_videoData.Fps = props.fps;
    m_videoData.VideoLocation = props.VideoLocation;


    m_cudaReader = VideoUtils::OpenStream(m_videoData.VideoLocation);
       
    return true;
}


bool VideoReader::Read()
{   

    m_cudaReader->nextFrame(m_frameGPU);

    return true;  
}

bool VideoReader::Show(cv::Mat frame) {
    if (frame.empty())
    {
        CORE_ERROR("VideoReader", "Show", "Frame is empty");
		return false;
    }

    m_lastFrame = frame;

    if (m_lastFrame .empty()) {
		CORE_ERROR("VideoReader", "Show", "Frame is empty");
    	return false;
    }

    cv::imshow(m_videoData.Title, m_lastFrame);
    return true;
}
