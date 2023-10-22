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
        std::string Name = "Hazel Application";
        std::string WorkingDirectory;
        ApplicationCommandLineArgs CommandLineArgs;
    };

    class Application
    {
    public:
        static Application* CreateApplication(ApplicationCommandLineArgs args)
        {
            ApplicationSpecification spec;
            spec.Name = "SoccerCamRecorder";
            spec.CommandLineArgs = args;

            return new Application(spec);
        }

        Application(const ApplicationSpecification& specification);
        virtual ~Application();

        void Close();

//        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        static Application& Get() { return *s_Instance; }

        const ApplicationSpecification& GetSpecification() const { return m_Specification; }

        void SubmitToMainThread(const std::function<void()>& function);
    private:
        void Run();
//        bool OnWindowClose(WindowCloseEvent& e);
//        bool OnWindowResize(WindowResizeEvent& e);

        void ExecuteMainThreadQueue();
    private:
        ApplicationSpecification m_Specification;
//        Scope<Window> m_Window;
//        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
//        LayerStack m_LayerStack;
        std::tm m_LastFrameTime = TimeUtils::GetCurrentTimestamp();

        std::vector<std::function<void()>> m_MainThreadQueue;
        std::mutex m_MainThreadQueueMutex;
    private:
        static Application* s_Instance;
        friend int ::main(int argc, char** argv);
    };

    // To be defined in CLIENTAPPLICATION


}