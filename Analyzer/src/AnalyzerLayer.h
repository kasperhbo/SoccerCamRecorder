#pragma once

#include <SoccerVisualization.h>

class AnalyzerLayer : public SoccerCamRecorder::Layer
{
public:
    AnalyzerLayer();
    ~AnalyzerLayer() override = default;

    //open next video
    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate() override;

};

