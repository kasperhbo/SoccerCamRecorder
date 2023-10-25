//
// Created by Kasper de Bruin on 22-10-2023.
//

#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafeatures2d.hpp>
#include <opencv2/shape/shape_transformer.hpp>
#include <opencv2/cudacodec.hpp>
#include <vector>
#include <chrono>
#include <iostream>
#include <cmath>

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

        static void InitializeBarrelGPUMap(
            const float distcofs[3],
            cv::Mat& cameraMatrix, cv::cuda::GpuMat& resultX, cv::cuda::GpuMat& resultY, bool releaseCameraMatrix = false,
            const int width = 4000, const int height = 4000) {
            cv::Mat distCoefs;
            try{
                 distCoefs = (cv::Mat_<double>(1, 5) << distcofs[0], distcofs[1], distcofs[2], 0, 0);
            }catch (const cv::Exception& e){
                CORE_ERROR(e.msg);
                throw e;
			}


            cv::Mat resultXCPU;
            cv::Mat resultYCPU;

            try {
                cv::initUndistortRectifyMap(
                    cameraMatrix,
                    distCoefs,
                    cv::Mat(),
                    cameraMatrix,
                    cv::Size(width, height),
                    CV_32FC1,
                    resultXCPU,
                    resultYCPU
                );
            }
            catch (const cv::Exception& e) {
                CORE_ERROR(e.msg);
                throw e;
            }

            try {
                resultX.upload(resultXCPU);
            }catch (const cv::Exception& e) {
				CORE_ERROR(e.msg);
				throw e;
			}

            try {
                resultY.upload(resultYCPU);
            }
            catch (const cv::Exception& e) {
                CORE_ERROR(e.msg);
                throw e;    
            }
            

            resultXCPU.release();
            resultYCPU.release();
            distCoefs.release();

   //         try {
   //             if (releaseCameraMatrix)
   //                 //cameraMatrix.release();s
   //         }catch (const cv::Exception& e) {
			//	CORE_ERROR(e.msg);
			//	throw e;
			//}
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

        static void Remap(cv::cuda::GpuMat& src, cv::cuda::GpuMat& dst, cv::cuda::GpuMat& mapX, cv::cuda::GpuMat& mapY) {
            try {
                cv::cuda::remap(src, dst, mapX, mapY, cv::INTER_LINEAR, cv::BORDER_CONSTANT);
            }
            catch (cv::Exception& e) {
                std::cout << e.what() << std::endl;
                std::cout << e.msg << std::endl;
                throw e;
            }
        }

        static void ShiftYMap(int shift, int originalWidth, int originalHeight, cv::cuda::GpuMat& resultX, cv::cuda::GpuMat& resultY) {
            cv::Mat M = (cv::Mat_<double>(2, 3) << 1, 0, 0, 0, 1, shift);

            cv::Size dsize = cv::Size(originalWidth, originalHeight + shift);
            try {
                cv::cuda::buildWarpAffineMaps(
                    M, false, dsize, resultX, resultY
                );
            }
            catch (cv::Exception& e) {
                std::cout << e.what() << std::endl;
            }

            M.release();
        }


        static void MakeWarpAffineMaps(
            const cv::Point2f srcPoints[3], const cv::Point2f dstPoints[3],
            cv::cuda::GpuMat& resultX, cv::cuda::GpuMat& resultY,
            const int imageWidth, const int imageHeight) {

            cv::Mat warpMatrixLeft;
            warpMatrixLeft = cv::getAffineTransform(srcPoints, dstPoints);

            cv::cuda::buildWarpAffineMaps(warpMatrixLeft, false, cv::Size(imageWidth, imageHeight), resultX, resultY);


            warpMatrixLeft.release();
        }

    };
}