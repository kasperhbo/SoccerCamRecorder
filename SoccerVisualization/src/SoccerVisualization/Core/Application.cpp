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
        CORE_INFO("Closing application");
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PopLayer(Layer* layer)
    {
        m_LayerStack.PopLayer(layer);
    }

    void Application::Run() {
        CORE_INFO("Running application");

        while (m_Running) {
            for (auto layer : m_LayerStack) {
                layer->OnUpdate();
            }            
        }
    }
};