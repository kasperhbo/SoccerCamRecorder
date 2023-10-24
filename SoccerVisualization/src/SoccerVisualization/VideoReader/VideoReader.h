//
// Created by Kasper de Bruin on 23-10-2023.
//

#pragma once

#include "SoccerVisualization.h"

#include <iostream>
#include <string>
#include <vector>

//import cv2
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudacodec.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudawarping.hpp>

namespace SoccerCamRecorder {
    struct VideoProps
    {
        std::string VideoLocation;
        uint32_t Width;
        uint32_t Height;
        uint32_t fps;

        VideoProps(
            const std::string& videoLocation = "",
            uint32_t originalWidth = 4000,
            uint32_t originalHeight= 3000,
            uint32_t fps = 25
            )
            : VideoLocation(videoLocation),
            Width(originalWidth), 
            Height(originalHeight), fps(fps)
        {
        }
    };

    class VideoReader {
    public:
        VideoReader(const VideoProps& props);
        ~VideoReader();

        void Terminate();

        bool Initialize(const VideoProps& props);
        bool Read();
        bool Show(cv::Mat frame);

        cv::cuda::GpuMat GetFrame() const { return m_frameGPU; };
        cv::Mat GetLastFrameCPU() const { return m_lastFrame; };

        std::string GetTitle() const { return m_videoData.Title; };
        std::string GetVideoLocation() const { return m_videoData.VideoLocation; };

        unsigned int GetWidth() const { return m_videoData.Width; };
        unsigned int GetHeight() const { return m_videoData.Height; };
		unsigned int GetFps() const { return m_videoData.Fps; };
        
        
        static Scope<VideoReader> Create(const VideoProps& props = VideoProps());


    private:
        cv::Ptr<cv::cudacodec::VideoReader> m_cudaReader;
        cv::cuda::GpuMat m_frameGPU;
        cv::Mat m_lastFrame;

        struct VideoData
        {
            std::string Title, VideoLocation;
            unsigned int Width, Height, Fps;
        };

        VideoData m_videoData;
    };
}

