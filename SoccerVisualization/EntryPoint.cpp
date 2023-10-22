////system
//#include <iostream>
//
////opencv
//#include <opencv2/opencv.hpp>
//
////base
//#include "Base.h"
//
////local
//#include "Utils/CudaUtils.h"
//#include "Utils/TimeUtils.h"
//#include "Utils/Log.h"
//#include "Utils/ImageUtils.h"

#pragma once
#include "Base.h"
#include "Application.h"

int main(int argc, char** argv){
    SoccerCamRecorder::Log::Init();

//    HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
     auto app = SoccerCamRecorder::Application::CreateApplication({ argc, argv });
//    HZ_PROFILE_END_SESSION();

//    HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
    app->Run();
//    HZ_PROFILE_END_SESSION();

//    HZ_PROFILE_BEGIN_SESSION("Shutdown", "HazelProfile-Shutdown.json");
    delete app;
//    HZ_PROFILE_END_SESSION();
}
