//
// Created by Kasper de Bruin on 22-10-2023.
//

#include <SoccerVisualization.h>
#include <SoccerVisualization/Core/EntryPoint.h>

#include "AnalyzerLayer.h"

class AnalyzerApplication : public SoccerCamRecorder::Application
{
public:
    AnalyzerApplication(const SoccerCamRecorder::ApplicationSpecification& specification)
            : SoccerCamRecorder::Application(specification)
    {
        Layer* layer = new AnalyzerLayer();
        PushLayer(layer);
    }

    ~AnalyzerApplication()
    {
    }
};


SoccerCamRecorder::Application* SoccerCamRecorder::CreateApplication(SoccerCamRecorder::ApplicationCommandLineArgs args)
{
    ApplicationSpecification spec;
    spec.Name = "Sandbox";
    spec.WorkingDirectory = "";
    spec.CommandLineArgs = args;

    return new AnalyzerApplication(spec);
}
