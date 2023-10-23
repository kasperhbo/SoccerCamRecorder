//
// Created by Kasper de Bruin on 23-10-2023.
//

#pragma once

#include "SoccerVisualization.h"
#include "SoccerVisualization/Core/Base.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace SoccerCamRecorder {
    struct WindowProps
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;

        WindowProps(const std::string& title = "Hazel Engine",
            uint32_t width = 1600,
            uint32_t height = 900)
            : Title(title), Width(width), Height(height)
        {
        }
    };

    class VideoReader {
    public:
        VideoReader(const WindowProps& props);
        ~VideoReader();

        void Terminate();

        bool Initialize(const WindowProps& props);
        bool Read();
        bool Show();


        static Scope<VideoReader> Create(const WindowProps& props = WindowProps());


    private:
        cv::VideoCapture capLeft;
        cv::Mat frameLeft;
        cv::Mat frameLeftResized;

        const std::string locationLeft;
    };
}

