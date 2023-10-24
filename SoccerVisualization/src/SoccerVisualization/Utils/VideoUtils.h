//
// Created by Kasper de Bruin on 23-10-2023.
//

#pragma once
#include "SoccerVisualization/Core/Base.h"

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

namespace SoccerCamRecorder {
    class VideoUtils {
    public:
        static void OpenStream(const std::string &path, cv::VideoCapture &reader) {
            try {
                reader = cv::VideoCapture(path);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }
        }

        static void OpenStream(const std::string &path, cv::VideoCapture &reader, const cv::Size &size) {
            try {
                reader = cv::VideoCapture(path);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }

            try {
                reader.set(cv::CAP_PROP_FRAME_WIDTH, size.width);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }

            try {
                reader.set(cv::CAP_PROP_FRAME_HEIGHT, size.height);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }
        }

        static void OpenStream(const std::string &path, cv::VideoCapture &reader, const cv::Size &size, int fps) {
            try {
                reader = cv::VideoCapture(path);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }
            try {
                reader.set(cv::CAP_PROP_FRAME_WIDTH, size.width);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }

            try {
                reader.set(cv::CAP_PROP_FRAME_HEIGHT, size.height);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }

            try {
                reader.set(cv::CAP_PROP_FPS, fps);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }
        }

        static cv::Ptr<cv::cudacodec::VideoReader> OpenStream(const std::string path){
            try {
                return cv::cudacodec::createVideoReader(path);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }
        }

        ///<summary>
        ///Create a video writer
        ///</summary>
        ///<param name="fileName">The name of the file to write to</param>
        ///<param name="frameSize">The size of the frames</param>
        ///<param name="writer">The writer to write to</param>
        ///<param name="codec">The codec to use</param>
        ///<param name="fps">The fps of the video</param>
        ///<param name="colorFormat">The color format of the video</param>
        ///<param name="encoderCallback">The encoder callback</param>
        ///<param name="stream">The stream to use</param>
        static void CreateVideoWriter(
                const std::string &fileName, const cv::Size frameSize,
                cv::Ptr<cv::cudacodec::VideoWriter> writer,
                const cv::cudacodec::Codec codec = cv::cudacodec::Codec::H264, const double fps = 25.0,
                const cv::cudacodec::ColorFormat colorFormat = cv::cudacodec::ColorFormat::BGRA,
                cv::Ptr<cv::cudacodec::EncoderCallback> encoderCallback = 0,
                const cv::cudacodec::Stream &stream = cv::cudacodec::Stream::Null()
        ) {
            try {
                writer = cv::cudacodec::createVideoWriter(fileName, frameSize, codec, fps, colorFormat, encoderCallback, stream);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }
        }

        //Getting properties of stream
        static void GetSize(cv::VideoCapture &stream, cv::Size &size) {
            try {
                size.width = stream.get(cv::CAP_PROP_FRAME_WIDTH);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }
            try {
                size.height = stream.get(cv::CAP_PROP_FRAME_HEIGHT);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }
        }

        static void GetSize(cv::Ptr<cv::cudacodec::VideoReader> &stream, cv::Size &size) {
            try {
                size.width = stream->format().width;
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }

            try {
                size.height = stream->format().height;
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }
        }

        static void GetSize(cv::VideoCapture &stream, int &width, int &height) {
            try {
                width = stream.get(cv::CAP_PROP_FRAME_WIDTH);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }

            try {
                height = stream.get(cv::CAP_PROP_FRAME_HEIGHT);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }
        }

        static cv::Size GetSize(cv::Ptr<cv::cudacodec::VideoReader> &stream) {
            try {
                return cv::Size(stream->format().width, stream->format().height);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }
        }

        //Getting frames
        static void GetFrame(cv::VideoCapture &stream, cv::Mat &frame) {
            try {
                stream >> frame;
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }
        }

        static void GetFrame(cv::VideoCapture &stream, cv::cuda::GpuMat &frame) {
            cv::Mat temp;

            try {
                stream >> temp;
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }

            try {
                frame.upload(temp);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
                throw e;
            }
        }

        static void GetFrame(cv::Ptr<cv::cudacodec::VideoReader> &stream, cv::cuda::GpuMat &frame) {
            try {
                stream->nextFrame(frame);
            } catch (cv::Exception &e) {
                CORE_ERROR(e.what());
            }
        };
    };
}