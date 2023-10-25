#pragma once
#include <iostream>

#include "SoccerVisualization/Utils/Log.h"
#include "SoccerVisualization/Utils/TimeUtils.h"
#include "SoccerVisualization/Utils/CudaUtil.h"
#include "SoccerVisualization/Utils/ImageUtil.h"
#include "SoccerVisualization/VideoProcessing/Soc_VideoReader.h"
#include "SoccerVisualization/VideoProcessing/Soc_VideoWriter.h"
#include "SoccerVisualization/VideoProcessing/Soc_VideoViewer.h"


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
	


class VideoProcessor
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="fileLeft">Left video file path</param>
	/// <param name="fileRight"> Right video file path</param>
	/// <param name="finalFile"> Final video file path</param>
	/// <param name="saveVideo"> Save the video</param>
	/// <param name="showVideo"> Show the video</param>
	/// <param name="fps">       The fps</param>
	/// <param name="width">     The target width</param>
	/// <param name="height">    The target height</param>
	VideoProcessor(
		const std::string fileLeft, const std::string fileRight, const std::string finalFile, 
		const bool saveVideo = false, const bool showVideo = true, const double fps = 25.0, const int width = 1920, const int height = 1080); 
	

	/// <summary>
	/// Destructor
	/// </summary>
	~VideoProcessor();

	/// <summary>
	/// Initialize the video processor
	/// </summary>
	/// <returns> If the video processor was initialized succesfully</returns>
	bool Initialize();
	

	/// <summary>
	/// Process the frame
	/// </summary>
	/// <returns>If the frame was processed succesfully</returns>
	bool ProcessFrame();


	/// <summary>
	/// Cleanup all the frames
	/// </summary>
	void ReleaseFrames();



	/// <summary>
	/// Cleanup all the GPU mats
	/// </summary>
	void ReleaseGPUMats();

	/// <summary>
	/// Cleanup all cpu mats
	/// </summary>
	void ReleaseCPUMats();

	/// <summary>
	/// Cleanup all the clips
	/// </summary>
	void ReleaseClips();

	/// <summary>
	/// Cleanup all the viewers
	/// </summary>
	void ReleaseViewers();		

	/// <summary>
	/// Cleanup all the writers
	/// </summary>
	void ReleaseReaders();


	/// <summary>
	/// Cleanup all the writers
	/// </summary>
	void ReleaseWriters();

	/// <summary>
	/// Terminate the video processor, and cleanup all the resources
	/// </summary>
	/// <returns></returns>
	bool Terminate();

private:

	/// <summary>
	/// Create the image maps, used for the barrel distortion, the shift maps, used for the position change
	/// </summary>
	void CreateImageMaps();

	/// <summary>
	/// Remove Barrel distortion
	/// </summary>
	void CreateBarrelUndistortMaps();

	/// <summary>
	/// Shift y map up
	/// </summary>
	void CreateShiftMap();

	/// <summary>
	/// Stretch the image top so we dont have black bars
	/// </summary>
	void CreatePointShiftMaps();

private:

	const int rightFrameYShift = 80;

	const float cofLeft[3] =  { -0.199f, -0.1300f, -0.0150f};
	const float cofRight[3] = { -0.4190f, 0.0780f, -0.0460f};
		
	const std::string fileLeft;
	const std::string fileRight;
	const std::string finalFile;

	const bool saveVideo = true;
	const bool showVideo = true;

	const double fps;
	const int finalWidth;
	const int finalHeight;

	int originalWidth;
	int originalHeight;

	cv::cuda::GpuMat frameLeft  = cv::cuda::GpuMat();
	cv::cuda::GpuMat frameRight = cv::cuda::GpuMat();;
	cv::cuda::GpuMat frameFinal = cv::cuda::GpuMat();;

	//Barrel distort maps
	cv::cuda::GpuMat barrelUndistortMapLeftX;// = cv::cuda::GpuMat();;
	cv::cuda::GpuMat barrelUndistortMapLeftY;// = cv::cuda::GpuMat();;

	cv::cuda::GpuMat barrelUndistortMapRightX;//= cv::cuda::GpuMat();;
	cv::cuda::GpuMat barrelUndistortMapRightY;//= cv::cuda::GpuMat();;

	//Shift maps
	cv::cuda::GpuMat shiftMapRightX ;//= cv::cuda::GpuMat();;
	cv::cuda::GpuMat shiftMapRightY ;//= cv::cuda::GpuMat();;
	
	//Position change maps
	cv::cuda::GpuMat positionChangeMapLeftX ;//= cv::cuda::GpuMat();;
	cv::cuda::GpuMat positionChangeMapLeftY; //0 = cv::cuda::GpuMat();;
	
	cv::cuda::GpuMat positionChangeMapRightX; // = cv::cuda::GpuMat();;
	cv::cuda::GpuMat positionChangeMapRightY ;// = cv::cuda::GpuMat();;
	

	Soc_VideoReader * videoReader;
	Soc_VideoViewer * videoViewer;	
	Soc_VideoWriter * videoWriter;	
};
