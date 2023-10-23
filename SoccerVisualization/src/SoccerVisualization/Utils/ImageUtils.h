//
// Created by Kasper de Bruin on 22-10-2023.
//

#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "CudaUtils.h"

namespace SoccerCamRecorder {

    class ImageUtils {
    public:
        static void ResizeImage(cv::Mat &src, cv::Mat &dst, const int width, const int height) {
            cv::resize(src, dst, cv::Size(width, height));
        }

        static void ResizeImage(cv::cuda::GpuMat &src, cv::cuda::GpuMat &dst, const int width, const int height) {
            return CudaUtils::ResizeImage(src, dst, width, height);
        }

        /// <summary>
        /// Concatenate two images horizontally
        /// </summary>
        /// <param name="left"></param>
        /// <param name="right"></param>
        /// <param name="result"></param>
        static void HConcat(const cv::Mat &left, const cv::Mat &right, cv::Mat &result) {
            cv::hconcat(left, right, result);
        }

        /// <summary>
        /// Concatenate two images horizontally
        /// </summary>
        /// <param name="left"></param>
        /// <param name="right"></param>
        /// <param name="result"></param>
        static void HConcat(const cv::cuda::GpuMat &left, const cv::cuda::GpuMat &right, cv::cuda::GpuMat &result) {
            return CudaUtils::HConcat(left, right, result);
        }

        /// <summary>
        /// Use this function to compare the resolution of the video frame with the resolution of the video viewer
        /// </summary>
        /// <param name="frame"></param>
        /// <returns>is same res</returns>
        static bool CheckResolution(cv::Mat &frame, int width, int height) {
            return frame.cols == width && frame.rows == height;
        }

        static bool CheckResolution(cv::cuda::GpuMat &frame, const int width, const int height) {
            return CudaUtils::CheckResolution(frame, width, height);
        }

        static void ShowImage(const cv::Mat &frame, const std::string &name) {
            cv::imshow(name, frame);
        }

        static void ShowImage(const cv::cuda::GpuMat &frame, const std::string &name) {
            CudaUtils::Show(frame, name);
        }
    };
}