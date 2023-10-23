//
// Created by Kasper de Bruin on 22-10-2023.
//
#pragma once
#include "SoccerVisualization/Core/Base.h"

namespace SoccerCamRecorder{
    class Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnEvent() {}

        inline const std::string& GetName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };
}