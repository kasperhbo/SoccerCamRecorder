//
// Created by Kasper de Bruin on 23-10-2023.
//

#pragma once

#include "SoccerVisualization.h"

#include <opencv2/opencv.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafeatures2d.hpp>
#include <opencv2/shape/shape_transformer.hpp>
#include <opencv2/cudacodec.hpp>
#include "opencv2/opencv_modules.hpp"
#include <opencv2/core.hpp>
#include <opencv2/cudacodec.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace SoccerCamRecorder {
    struct VideoProps
    {
        std::string Title;
        std::string VideoLocation;
        uint32_t Width;
        uint32_t Height;

        VideoProps(
            const std::string& title = "Video Analyzer",
            const std::string& videoLocation,
            uint32_t width = 1600,
            uint32_t height = 900
            )
            : Title(title), VideoLocation(videoLocation), Width(width), Height(height)
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
        bool Show();

        cv::cuda::GpuMat getFrame() const { return m_frame; };

        static Scope<VideoReader> Create(const VideoProps& props = VideoProps());


    private:
        cv::Ptr<cv::cudacodec::VideoReader> m_cudaReader;
        cv::cuda::GpuMat m_frame;
    };
}

