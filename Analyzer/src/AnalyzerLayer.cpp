//
// Created by Kasper de Bruin on 22-10-2023.
//

#include "AnalyzerLayer.h"
#include "SoccerVisualization/Utils/TimeUtils.h"
#include "SoccerVisualization/VideoProcessing/Soc_VideoProcessor.h"
#include <chrono>
#include <imgui/imgui.h>


AnalyzerLayer::AnalyzerLayer()
: Layer("Example")
{
    CL_WARN("ExampleLayer::ExampleLayer constructor");
}

void AnalyzerLayer::OnAttach() {
    CL_WARN("ExampleLayer::OnAttach");
	
}

void AnalyzerLayer::OnUpdate()
{    
    CL_TRACE("ExampleLayer::OnUpdate");	
}

void AnalyzerLayer::OnDetach() {
    CL_TRACE("ExampleLayer::OnDetach");
}

