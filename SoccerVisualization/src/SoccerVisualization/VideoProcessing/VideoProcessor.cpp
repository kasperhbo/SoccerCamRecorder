#include "VideoProcessor.h"
#include "SoccerVisualization/Utils/ImageUtils.h"

Scope<VideoProcessor> VideoProcessor::Create(
	const VideoProcessorPropsFinal& propsFinal,
	const VideoProcessorProps& propsLeft, const VideoProcessorProps& propsRight)
{
	return CreateScope<VideoProcessor>(propsFinal, propsLeft, propsRight);
}



VideoProcessor::VideoProcessor(
	const VideoProcessorPropsFinal& propsFinal,
	const VideoProcessorProps& propsLeft, const VideoProcessorProps& propsRight)
{
	Initialize(propsFinal, propsLeft, propsRight);
}

VideoProcessor::~VideoProcessor()
{

}

bool SoccerCamRecorder::VideoProcessor::ProcessFrames(cv::cuda::GpuMat& frameLeft, cv::cuda::GpuMat& frameRight)
{

	//Undistort frames
	try {
		//Undistort frames
		ImageUtils::Remap(frameLeft,  frameLeft,  m_barrelUndistortMapLeftX, m_barrelUndistortMapLeftY);
		ImageUtils::Remap(frameRight, frameRight, m_barrelUndistortMapRightX, m_barrelUndistortMapRightY);

		//Shift right frame
		ImageUtils::Remap(frameRight, frameRight, m_shiftMapRightX, m_shiftMapRightY);

		//Shift frame points
		ImageUtils::Remap(frameLeft, frameLeft, positionChangeMapLeftX, positionChangeMapLeftY);
		ImageUtils::Remap(frameRight, frameRight, positionChangeMapRightX, positionChangeMapRightY);
	}
	catch (const cv::Exception & e) {
		CORE_ERROR("Failed to undistort frames: {0}", e.what());
		return false;
	}

	//Concetonate frames
	try {
		ImageUtils::HConcat(frameLeft, frameRight, m_finalFrame);
	}
	catch (const cv::Exception & e) {
		CORE_ERROR("Failed to concatenate frames: {0}", e.what());
		return false;
	}

	//Resize image
	try {
		ImageUtils::ResizeImage(
			m_finalFrame, m_finalFrame, 
			m_videoProcessorPropsFinal.finalWidth, m_videoProcessorPropsFinal.finalHeight);
	}catch (const cv::Exception & e) {
		CORE_ERROR("Failed to resize frames: {0}", e.what());
		return false;
	}

	return true;
}

bool VideoProcessor::Initialize(
	const VideoProcessorPropsFinal& propsFinal,
	const VideoProcessorProps& propsLeft, const VideoProcessorProps& propsRight)
{
	m_videoProcessorPropsFinal = propsFinal;
	m_videoProcessorPropsLeft = propsLeft;
	m_videoProcessorPropsRight = propsRight;

	if(!InitializeMaps()){
		return false;
	}

	return true;
}

bool VideoProcessor::InitializeMaps()
{
	if (!InitializeMapsLeft()) {
		return false;
	}
	if (!InitializeMapsRight()) {
		return false;
	}
	CreatePointShiftMaps();
	return true;
}

bool VideoProcessor::InitializeMapsLeft()
{
	try {
		ImageUtils::InitializeBarrelGPUMap(
			m_videoProcessorPropsLeft.cof,
			m_videoProcessorPropsLeft.cameraMatrix,
			m_barrelUndistortMapLeftX,
			m_barrelUndistortMapLeftY,
			true
		);
	}
	catch (const cv::Exception& e) {
		CORE_ERROR("Failed to initialize barrel undistort map for left camera: {0}", e.what());
		return false;
	}
	return true;
}

bool VideoProcessor::InitializeMapsRight()
{
	//Barrel
	try {
		ImageUtils::InitializeBarrelGPUMap(
			m_videoProcessorPropsRight.cof,
			m_videoProcessorPropsRight.cameraMatrix,
			m_barrelUndistortMapRightX,
			m_barrelUndistortMapRightY,
			true
		);
	}
	catch (const cv::Exception& e) {
		CORE_ERROR("Failed to initialize barrel undistort map for right camera: {0}", e.what());
		return false;
	}

	//Shift
	ImageUtils::ShiftYMap(m_videoProcessorPropsRight.shiftY, m_videoProcessorPropsRight.width, m_videoProcessorPropsRight.height, m_shiftMapRightX, m_shiftMapRightY);
	return true;
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
		cv::Point2f(0, (float)m_videoProcessorPropsLeft.height),//bottom left
		cv::Point2f((float)m_videoProcessorPropsLeft.width, 0),//top right
	};

	cv::Point2f dstPointsLeft[3] = {
		cv::Point2f(0, 0 + shiftTotalHeightAdd)        , //top left
		cv::Point2f(0, (float)m_videoProcessorPropsLeft.height - offsetMidfield),//bottom right
		cv::Point2f((float)m_videoProcessorPropsLeft.width, midShiftLeft + shiftTotalHeightAdd)//top right
	};

	ImageUtils::MakeWarpAffineMaps(srcPointsLeft, dstPointsLeft, positionChangeMapLeftX, positionChangeMapLeftY, m_videoProcessorPropsLeft.width, m_videoProcessorPropsLeft.height);

	//Right
	cv::Point2f srcPointsRight[3] = {
		cv::Point2f(0, 0),//top left
		cv::Point2f(0, (float)m_videoProcessorPropsRight.height),//bottom left
		cv::Point2f((float)m_videoProcessorPropsRight.width, 0),//top right
	};

	cv::Point2f dstPointsRight[3] = {
		cv::Point2f(0, midShiftRightTopLeft + shiftTotalHeightAdd),//top left
		cv::Point2f(0, (float)m_videoProcessorPropsRight.height + midShiftRightBottomLeft),//bottom left
		cv::Point2f((float)m_videoProcessorPropsRight.width, shiftTotalHeightAdd)//top right
	};

	ImageUtils::MakeWarpAffineMaps(srcPointsRight, dstPointsRight, positionChangeMapRightX, positionChangeMapRightY, m_videoProcessorPropsRight.width, m_videoProcessorPropsRight.height);
}