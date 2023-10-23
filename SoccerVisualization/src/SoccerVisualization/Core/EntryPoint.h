#pragma once

#include "Base.h"
#include "Application.h"

extern SoccerCamRecorder::Application* SoccerCamRecorder::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv) {
    SoccerCamRecorder::Log::Init();
    auto app= SoccerCamRecorder::CreateApplication({ argc, argv });
    app->Run();
    delete app;
}

