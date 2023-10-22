//
// Created by Kasper de Bruin on 22-10-2023.
//

#pragma once

#include "../Base.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)


namespace SoccerCamRecorder {
    class Log
    {
        public:
            static void Init();

            static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
            static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
        private:
            static Ref<spdlog::logger> s_CoreLogger;
            static Ref<spdlog::logger> s_ClientLogger;
    };
}

// Core log macros
#define SCRCAM_CORE_TRACE(...)    ::SoccerCamRecorder::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SCRCAM_CORE_INFO(...)     ::SoccerCamRecorder::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SCRCAM_CORE_WARN(...)     ::SoccerCamRecorder::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SCRCAM_CORE_ERROR(...)    ::SoccerCamRecorder::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SCRCAM_CORE_CRITICAL(...) ::SoccerCamRecorder::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define SCRCAM_TRACE(...)         ::SoccerCamRecorder::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SCRCAM_INFO(...)          ::SoccerCamRecorder::Log::GetClientLogger()->info(__VA_ARGS__)
#define SCRCAM_WARN(...)          ::SoccerCamRecorder::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SCRCAM_ERROR(...)         ::SoccerCamRecorder::Log::GetClientLogger()->error(__VA_ARGS__)
#define SCRCAM_CRITICAL(...)      ::SoccerCamRecorder::Log::GetClientLogger()->critical(__VA_ARGS__)