//
// Created by Kasper de Bruin on 22-10-2023.
//

#include "Application.h"

namespace SoccerCamRecorder {

    Application *Application::s_Instance = nullptr;

    Application::Application(const SoccerCamRecorder::ApplicationSpecification &specification) {
        s_Instance = this;
    }

    Application::~Application() {
        s_Instance = nullptr;
    }

    void Application::Close() {
        SCRCAM_CORE_INFO("Closing application");
    }

    void Application::Run() {
        SCRCAM_CORE_INFO("Running application");

        while (m_Running) {
            std::tm currentTime = TimeUtils::GetCurrentTimestamp();
            double timeStep = TimeUtils::GetTimeDifference(m_LastFrameTime,currentTime);
            SCRCAM_WARN("Time step: {0}", timeStep);
            m_LastFrameTime = currentTime;
        }
    }
};