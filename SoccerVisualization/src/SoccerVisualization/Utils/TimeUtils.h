//
// Created by Kasper de Bruin on 22-10-2023.
//

#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>

namespace SoccerCamRecorder {
    class TimeUtils {

    public:
        static std::tm GetCurrentTimestamp() {
            // Return the current timestamp as a formatted string
            // Get the current time point
            auto now = std::chrono::system_clock::now();

            // Convert it to a time_t object
            std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

            // Use a safer method to get localtime based on the platform
            std::tm localTime{};

#if defined(_WIN32) || defined(_WIN64)  // Windows-specific code
            localtime_s(&localTime, &currentTime);
#else  // POSIX-specific code
            localtime_r(&currentTime, &localTime);
#endif
            return localTime;
        }

        static std::string GetFormattedString() {
            std::tm localTime = GetCurrentTimestamp();

            // Use a stringstream to format the time into a string suitable for filenames
            std::stringstream ss;

            ss << localTime.tm_year + 1900 << "-"
               << localTime.tm_mon + 1 << "-"
               << localTime.tm_mday << "_"
               << localTime.tm_hour << "-"
               << localTime.tm_min << "-"
               << localTime.tm_sec;
            // Return the formatted time string
            return ss.str();
        }

        static double GetTimeDifference(std::tm begin, std::tm end) {
            std::tm tempBegin = begin;
            std::tm tempEnd = end;

            double beginSeconds = tempBegin.tm_hour * 3600 + tempBegin.tm_min * 60 + tempBegin.tm_sec;
            double endSeconds = tempEnd.tm_hour * 3600 + tempEnd.tm_min * 60 + tempEnd.tm_sec;

            return endSeconds - beginSeconds;
        }
    };
}