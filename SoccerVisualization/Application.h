//
// Created by Kasper de Bruin on 22-10-2023.
//

#pragma once

#include "Base.h"
#include "Utils/Log.h"
#include "Utils/TimeUtils.h"

int main(int argc, char** argv);

namespace SoccerCamRecorder {

    struct ApplicationCommandLineArgs
    {
        int Count = 0;
        char** Args = nullptr;

        const char* operator[](int index) const
        {
//            HZ_CORE_ASSERT(index < Count);
            return Args[index];
        }
    };

    struct ApplicationSpecification
    {
        std::string Name = "APPLICATIONTITLE";
        std::string WorkingDirectory;
        ApplicationCommandLineArgs CommandLineArgs;
    };

    class Application
    {
    public:
        Application(const ApplicationSpecification& specification);
        virtual ~Application();

        void Close();

        static Application& Get() { return *s_Instance; }

        const ApplicationSpecification& GetSpecification() const { return m_Specification; }

        void SubmitToMainThread(const std::function<void()>& function);

    private:
        void Run();
    private:
        ApplicationSpecification m_Specification;
        bool m_Running = true;
        bool m_Minimized = false;
        std::tm m_LastFrameTime = TimeUtils::GetCurrentTimestamp();

        std::vector<std::function<void()>> m_MainThreadQueue;
        std::mutex m_MainThreadQueueMutex;
    private:
        static Application* s_Instance;
        friend int ::main(int argc, char** argv);
    };

//    Application* CreateApplication(ApplicationCommandLineArgs args)
//    {
//        ApplicationSpecification spec;
//        spec.Name = "Soccer cam recorder";
//        spec.CommandLineArgs = args;
//
//        return new Application(spec);
//    }

}