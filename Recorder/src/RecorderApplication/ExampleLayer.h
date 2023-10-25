#pragma once

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

};

