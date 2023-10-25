#include "VideoProcessor.h"


VideoProcessor::VideoProcessor(const std::string fileLeft, const std::string fileRight, const std::string finalFile, const bool saveVideo, const bool showVideo, const double fps, const int width, const int height)
	: fileLeft(fileLeft), fileRight(fileRight), finalFile(finalFile), saveVideo(saveVideo), showVideo(showVideo), fps(fps), finalWidth(width), finalHeight(height)
{

}

VideoProcessor::~VideoProcessor()
{
	Terminate();
}

bool VideoProcessor::Initialize()
{

	CORE_INFO(
		"VideoProcessor: Initializing. FileLeft: {0}, FileRight: {1}, FinalFile: {2}, SaveVideo: {3}, ShowVideo: {4}, FPS: {5}, Width: {6}, Height: {7}",
		fileLeft, fileRight, finalFile, saveVideo, showVideo, fps, finalWidth, finalHeight);

	videoReader = new Soc_VideoReader(fileLeft, fileRight);

	if (videoReader->Initialize())
	{
		CORE_INFO("Soc_VideoReader: Initialized {} ");

		cv::cuda::GpuMat* firstFrameLeft = new cv::cuda::GpuMat();
		cv::cuda::GpuMat* firstFrameRight = new cv::cuda::GpuMat();

		videoReader->Read(*firstFrameLeft, *firstFrameRight);

		originalWidth = firstFrameLeft->cols;
		originalHeight = firstFrameRight->rows;

		firstFrameLeft->release();
		firstFrameRight->release();

		CORE_INFO("Original width: {0}, original height: {1}", originalWidth, originalHeight);
	}
	else
	{
		CORE_CRITICAL("Soc_VideoReader: Failed to initialize");
		return false;
	}

	if (saveVideo)
	{
		videoWriter = new Soc_VideoWriter(finalFile, finalWidth, finalHeight, fps);

		if (videoWriter->Initialize())
		{
			CORE_INFO("Soc_VideoWriter: Initialized");
		}
		else
		{
			CORE_CRITICAL("Soc_VideoWriter: Failed to initialize");
			return false;
		}
	}
	if (showVideo)
	{
		videoViewer = new Soc_VideoViewer(finalWidth, finalHeight);
		if (videoViewer->Initialize())
		{
			CORE_INFO("Soc_VideoViewer: Initialized");
		}
		else
		{
			CORE_CRITICAL("Soc_VideoViewer: Failed to initialize");
			return false;
		}
	}

	CreateImageMaps();

	CORE_INFO("VideoProcessor: Initialized");
	return true;
}

bool VideoProcessor::ProcessFrame()
{
	if (videoReader->Read(frameLeft, frameRight))
	{
		CORE_INFO(
			"VideoProcessor: Processig.... FileLeft: {0}, FileRight: {1}, FinalFile: {2}, SaveVideo: {3}, ShowVideo: {4}, FPS: {5}, Width: {6}, Height: {7}",
			fileLeft, fileRight, finalFile, saveVideo, showVideo, fps, finalWidth, finalHeight);
		
		int64 startTicks = cv::getTickCount();

		//Undistort frames
		CudaUtil::Remap(frameLeft, frameLeft, barrelUndistortMapLeftX, barrelUndistortMapLeftY);
		CudaUtil::Remap(frameRight, frameRight, barrelUndistortMapRightX, barrelUndistortMapRightY);

		//Shift right frame
		CudaUtil::Remap(frameRight, frameRight, shiftMapRightX, shiftMapRightY);

		//Shift frame points
		CudaUtil::Remap(frameLeft, frameLeft, positionChangeMapLeftX, positionChangeMapLeftY);
		CudaUtil::Remap(frameRight, frameRight, positionChangeMapRightX, positionChangeMapRightY);

		//Concat frames
		ImageUtil::HConcat(frameLeft, frameRight, frameFinal);

		//Resize the image to the output width and height
		ImageUtil::ResizeImage(frameFinal, frameFinal, finalWidth, finalHeight);

		//Write the frame
		if (saveVideo)
		{
			videoWriter->Write(frameFinal);
		}

		if (showVideo)
		{
			videoViewer->ShowVideo(frameFinal);
		}

		
		int64 endTicks = cv::getTickCount(); // end tick count
		double timeInSeconds = (double)(endTicks - startTicks) / cv::getTickFrequency();

		CORE_INFO("VideoProcessor: Finished processing, frame time was: {0}", timeInSeconds);
		return true;
	}
	else
	{
		CORE_INFO("VideoProcessor: Finished processing");
		return false;
	}



}

void VideoProcessor::ReleaseFrames()
{
	frameLeft.release();
	frameRight.release();
	frameFinal.release();
}




void VideoProcessor::ReleaseViewers()
{
	videoViewer->Terminate();
}

void VideoProcessor::ReleaseReaders()
{
	videoReader->Terminate();
}

void VideoProcessor::ReleaseWriters()
{
	videoWriter->Terminate();
}


void VideoProcessor::ReleaseGPUMats() {

	barrelUndistortMapLeftX.release();
	barrelUndistortMapLeftY.release();
							   
	barrelUndistortMapRightX.release();
	barrelUndistortMapRightY.release();

	shiftMapRightX.release();
	shiftMapRightY.release();

	positionChangeMapLeftX.release();
	positionChangeMapLeftY.release();

	positionChangeMapRightX.release();
	positionChangeMapRightY.release();
}

void VideoProcessor::ReleaseCPUMats() {

}


bool VideoProcessor::Terminate()
{
	void ReleaseFrames();
	void ReleaseClips();
	void ReleaseGPUMats();
	void ReleaseCPUMats();
	return true;
}

void VideoProcessor::CreateImageMaps() {
	CreateBarrelUndistortMaps();
	CreateShiftMap();
	CreatePointShiftMaps();
}

void VideoProcessor::CreateBarrelUndistortMaps()
{
	cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 3000, 0, 4000 / 2, 0, 3000, 3000 / 2, 0, 0, 1);
	//barrel undistort
	CudaUtil::InitializeBarrelGPUMap(cofLeft, cameraMatrix, barrelUndistortMapLeftX, barrelUndistortMapLeftY);
	CudaUtil::InitializeBarrelGPUMap(cofRight, cameraMatrix, barrelUndistortMapRightX, barrelUndistortMapRightY, true);
	
}

void VideoProcessor::CreateShiftMap() {
	//right
	CORE_INFO("VideoProcessor: Creating shift map");
	CudaUtil::MakeShiftYMap(rightFrameYShift, originalWidth, originalHeight, shiftMapRightX, shiftMapRightY);
}

void VideoProcessor::CreatePointShiftMaps() {
	//Position change
	float midShiftLeft = 310;
	float midShiftRightTopLeft = 260;
	float midShiftRightBottomLeft = 180;
	float shiftTotalHeightAdd = -700;

	//Left
	int offsetMidfield = 0;
	cv::Point2f srcPointsLeft[3] = {
		cv::Point2f(0, 0),//top left
		cv::Point2f(0, (float)originalHeight),//bottom left
		cv::Point2f((float)originalWidth, 0),//top right
	};

	cv::Point2f dstPointsLeft[3] = {
		cv::Point2f(0, 0 + shiftTotalHeightAdd)        , //top left
		cv::Point2f(0, (float)originalHeight - offsetMidfield),//bottom right
		cv::Point2f((float)originalWidth, midShiftLeft + shiftTotalHeightAdd)//top right
	};

	CudaUtil::MakeWarpAffineMaps(srcPointsLeft, dstPointsLeft, positionChangeMapLeftX, positionChangeMapLeftY, originalWidth, originalHeight);

	//Right
	cv::Point2f srcPointsRight[3] = {
		cv::Point2f(0, 0),//top left
		cv::Point2f(0, (float)originalHeight),//bottom left
		cv::Point2f((float)originalWidth, 0),//top right
	};

	cv::Point2f dstPointsRight[3] = {
		cv::Point2f(0, midShiftRightTopLeft + shiftTotalHeightAdd),//top left
		cv::Point2f(0, (float)originalHeight + midShiftRightBottomLeft),//bottom left
		cv::Point2f((float)originalWidth, shiftTotalHeightAdd)//top right
	};

	CudaUtil::MakeWarpAffineMaps(srcPointsRight, dstPointsRight, positionChangeMapRightX, positionChangeMapRightY, originalWidth, originalHeight);

}