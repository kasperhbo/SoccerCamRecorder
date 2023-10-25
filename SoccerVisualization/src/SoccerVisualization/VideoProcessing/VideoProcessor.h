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

namespace SoccerCamRecorder
{
	struct VideoProcessorProps {
		float cof[3];
		cv::Mat cameraMatrix;
		uint32_t width;
		uint32_t height;
		int shiftY;

		VideoProcessorProps(
			const std::array <float, 3>& c = { 0.0f, 0.0f, 0.0f },
			const cv::Mat& cameraMatrix = cv::Mat(),
			uint32_t width = 4000,
			uint32_t height = 3000,
			int shiftY = 0
		)
		{
			//std::copy(std::begin(c), std::end(c), std::begin(cof));
			cof[0] = c[0];
			cof[1] = c[1];
			cof[2] = c[2];
			
			cameraMatrix.copyTo(this->cameraMatrix);
			
			this->width = width;
			this->height = height;
			this->shiftY = shiftY;
		}
	};

	struct VideoProcessorPropsFinal {
		uint32_t finalWidth;
		uint32_t finalHeight;
		uint fps;

		VideoProcessorPropsFinal(
			uint32_t width = 4000,
			uint32_t height = 3000,
			uint fps = 25
		) : finalWidth(width), finalHeight(height), fps(fps)
		{
		}
	};

	class VideoProcessor
	{
	public:
		VideoProcessor(
			const VideoProcessorPropsFinal& propsFinal,
			const VideoProcessorProps& propsleft, const VideoProcessorProps& propsRight);
		~VideoProcessor();

		void Terminate();

		bool Initialize(
			const VideoProcessorPropsFinal& propsFinal,
			const VideoProcessorProps& propsleft, const VideoProcessorProps& propsRight);

		bool ProcessFrames(cv::cuda::GpuMat& frameLeft, cv::cuda::GpuMat& frameRight);

		cv::cuda::GpuMat GetFinalFrame() const { return m_finalFrame; };

		uint GetFps() const { return m_videoProcessorPropsFinal.fps; };
		uint GetFinalWidth() const { return m_videoProcessorPropsFinal.finalWidth; };
		uint GetFinalHeight() const { return m_videoProcessorPropsFinal.finalHeight; };

		static Scope<VideoProcessor> Create(
			const VideoProcessorPropsFinal& propsFinal,
			const VideoProcessorProps& propsLeft = VideoProcessorProps(), const VideoProcessorProps& propsRight = VideoProcessorProps());

	private:
		/// <summary>
		/// All functions seperate for error checking in the future
		/// </summary>
		/// <returns></returns>
		bool InitializeMaps();
		bool InitializeMapsLeft();
		bool InitializeMapsRight();
		void CreatePointShiftMaps();


	private:

		VideoProcessorPropsFinal m_videoProcessorPropsFinal;
		VideoProcessorProps m_videoProcessorPropsLeft;
		VideoProcessorProps m_videoProcessorPropsRight;
		
		//UNDISTORT MAPS
		//Barrel distort maps
		cv::cuda::GpuMat m_barrelUndistortMapLeftX;// = cv::cuda::GpuMat();;
		cv::cuda::GpuMat m_barrelUndistortMapLeftY;// = cv::cuda::GpuMat();;

		cv::cuda::GpuMat m_barrelUndistortMapRightX;//= cv::cuda::GpuMat();;
		cv::cuda::GpuMat m_barrelUndistortMapRightY;//= cv::cuda::GpuMat();;

		//Shift maps
		cv::cuda::GpuMat m_shiftMapRightX;//= cv::cuda::GpuMat();;
		cv::cuda::GpuMat m_shiftMapRightY;//= cv::cuda::GpuMat();;

		//Position change maps
		cv::cuda::GpuMat positionChangeMapLeftX;//= cv::cuda::GpuMat();;
		cv::cuda::GpuMat positionChangeMapLeftY; //0 = cv::cuda::GpuMat();;

		cv::cuda::GpuMat positionChangeMapRightX; // = cv::cuda::GpuMat();;
		cv::cuda::GpuMat positionChangeMapRightY;// = cv::cuda::GpuMat();;

		cv::cuda::GpuMat m_finalFrame;
	};
}
