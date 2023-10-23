//
// Created by Kasper de Bruin on 22-10-2023.
//

#include <SoccerVisualization.h>
#include <SoccerVisualization/Core/EntryPoint.h>

#include "ExampleLayer.h"

class RecorderApplication : public SoccerCamRecorder::Application
{
public:
    RecorderApplication(const SoccerCamRecorder::ApplicationSpecification& specification)
            : SoccerCamRecorder::Application(specification)
    {
        Layer* layer = new ExampleLayer();
        PushLayer(layer);
    }

    ~RecorderApplication()
    {
    }
};


SoccerCamRecorder::Application* SoccerCamRecorder::CreateApplication(SoccerCamRecorder::ApplicationCommandLineArgs args)
{
    ApplicationSpecification spec;
    spec.Name = "Sandbox";
    spec.WorkingDirectory = "";
    spec.CommandLineArgs = args;

    return new RecorderApplication(spec);
}
