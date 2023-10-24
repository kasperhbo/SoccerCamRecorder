//
// Created by Kasper de Bruin on 23-10-2023.
//
#include "VideoReader.h"

#include "../Utils/Log.h"
#include "SoccerVisualization/Utils/VideoUtils.h"

Scope<VideoReader> VideoReader::Create(const VideoProps& props)
{
    return CreateScope<VideoReader>(props);
}

VideoReader::VideoReader(const VideoProps& props) { 
	CORE_INFO("VideoReader", "VideoReader", "VideoReader created");
    Initialize(props);
}


VideoReader::~VideoReader()
{
	CORE_INFO("VideoReader", "~VideoReader", "VideoReader destroyed");
    //delete frame;
}

void VideoReader::Terminate(){
	CORE_INFO("VideoReader", "Terminate", "VideoReader terminated");

}

bool VideoReader::Initialize(const VideoProps& props) {
    CORE_INFO("VideoReader", "Initialize", "VideoReader initialized");

    m_cudaReader = VideoUtils::OpenStream(props.VideoLocation);
    
        
    return true;
}


bool VideoReader::Read()
{   
    CORE_INFO("VideoReader", "Read", "VideoReader read frame");


    return true;
    
}

bool VideoReader::Show() {
    CORE_INFO("VideoReader", "Show", "VideoReader show frame");

    return true;
}
