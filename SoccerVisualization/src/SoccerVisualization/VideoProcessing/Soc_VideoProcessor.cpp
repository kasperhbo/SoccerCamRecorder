#include "Soc_VideoProcessor.h"


Soc_VideoProcessor::Soc_VideoProcessor(const std::string fileLeft, const std::string fileRight, const std::string finalFile, const bool saveVideo, const bool showVideo, const double fps, const int width, const int height)
	: fileLeft(fileLeft), fileRight(fileRight), finalFile(finalFile), saveVideo(saveVideo), showVideo(showVideo), fps(fps), finalWidth(width), finalHeight(height)
{

}

Soc_VideoProcessor::~Soc_VideoProcessor()
{
	Terminate();
}

bool Soc_VideoProcessor::Initialize()
{

	CORE_INFO(
		"Soc_VideoProcessor: Initializing. FileLeft: {0}, FileRight: {1}, FinalFile: {2}, SaveVideo: {3}, ShowVideo: {4}, FPS: {5}, Width: {6}, Height: {7}",
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

	CORE_INFO("Soc_VideoProcessor: Initialized");
	return true;
}

bool Soc_VideoProcessor::ProcessFrame()
{
	if (videoReader->Read(frameLeft, frameRight))
	{
		CORE_INFO(
			"Soc_VideoProcessor: Processig.... FileLeft: {0}, FileRight: {1}, FinalFile: {2}, SaveVideo: {3}, ShowVideo: {4}, FPS: {5}, Width: {6}, Height: {7}",
			fileLeft, fileRight, finalFile, saveVideo, showVideo, fps, finalWidth, finalHeight);
		
		int64 startTicks = cv::getTickCount();

		//Remove Barrel Distortion frames
		CudaUtil::Remap(frameLeft, frameLeft, barrelUndistortMapLeftX, barrelUndistortMapLeftY);
		CudaUtil::Remap(frameRight, frameRight, barrelUndistortMapRightX, barrelUndistortMapRightY);

		//Shift right frame
		CudaUtil::Remap(frameRight, frameRight, shiftMapRightX, shiftMapRightY);

		//Shift frame points
		CudaUtil::Remap(frameLeft, frameLeft, positionChangeMapLeftX, positionChangeMapLeftY);
		CudaUtil::Remap(frameRight, frameRight, positionChangeMapRightX, positionChangeMapRightY);

		//Concatenate frames
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

		CORE_INFO("Soc_VideoProcessor: Finished processing, frame time was: {0}", timeInSeconds);
		return true;
	}
	else
	{
		CORE_INFO("Soc_VideoProcessor: Finished processing");
		return false;
	}
}

void Soc_VideoProcessor::ReleaseFrames()
{
	frameLeft.release();
	frameRight.release();
	frameFinal.release();
}

void Soc_VideoProcessor::ReleaseViewers()
{
	videoViewer->Terminate();
}

void Soc_VideoProcessor::ReleaseReaders()
{
	videoReader->Terminate();
}

void Soc_VideoProcessor::ReleaseWriters()
{
	videoWriter->Terminate();
}

void Soc_VideoProcessor::ReleaseGPUMats() {

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

void Soc_VideoProcessor::ReleaseCPUMats() {

}

bool Soc_VideoProcessor::Terminate()
{
	void ReleaseFrames();
	void ReleaseClips();
	void ReleaseGPUMats();
	void ReleaseCPUMats();
	return true;
}

void Soc_VideoProcessor::CreateImageMaps() {
	CreateBarrelUndistortMaps();
	CreateShiftMap();
	CreatePointShiftMaps();
}

void Soc_VideoProcessor::CreateBarrelUndistortMaps()
{
	cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 3000, 0, 4000 / 2, 0, 3000, 3000 / 2, 0, 0, 1);
	//barrel undistort
	CudaUtil::InitializeBarrelGPUMap(cofLeft, cameraMatrix, barrelUndistortMapLeftX, barrelUndistortMapLeftY);
	CudaUtil::InitializeBarrelGPUMap(cofRight, cameraMatrix, barrelUndistortMapRightX, barrelUndistortMapRightY, true);
	
}

void Soc_VideoProcessor::CreateShiftMap() {
	//right
	CORE_INFO("Soc_VideoProcessor: Creating shift map");
	CudaUtil::MakeShiftYMap(rightFrameYShift, originalWidth, originalHeight, shiftMapRightX, shiftMapRightY);
}

void Soc_VideoProcessor::CreatePointShiftMaps() {
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