#pragma once

#include "SoccerVisualization/VideoReader/VideoReader.h"
#include <SoccerVisualization.h>

class ExampleLayer : public SoccerCamRecorder::Layer
{
public:
    ExampleLayer();
    ~ExampleLayer() override = default;

    //open next video
    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate() override;



private:


private:
    //SoccerCamRecorder::VideoReader videoReader;
    Scope<SoccerCamRecorder::VideoReader> videoReader;
};

