#pragma once

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

#include "SoccerVisualization/Utils/ImageUtil.h"

using namespace std;

class Soc_VideoWriter
{
public:
	Soc_VideoWriter(cv::String location, int width, int height, double fps);
	~Soc_VideoWriter();

	void Terminate();

	bool Initialize();


	/// <summary>
	/// Write the two frames to the video as one frame
	/// </summary>
	/// <param name="frameLeft"></param>
	/// <param name="frameRight"></param>
	/// <param name="result"></param>
	void Write(cv::cuda::GpuMat& frameToShow);

private:
	const int width;
	const int height;
	const cv::String location;
	const double fps;

	cv::Ptr<cv::cudacodec::VideoWriter> d_writer;

	cv::cuda::Stream stream; // Stream for asynchronous operations
	
};