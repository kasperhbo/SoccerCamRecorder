#pragma once

#include "Base.h"
#include "Application.h"

int main(int argc, char **argv) {
    SoccerCamRecorder::Log::Init();
    ApplicationSpecification spec;
    spec.Name = "Soccer cam recorder";
    spec.CommandLineArgs = {argc, argv};

    auto app = SoccerCamRecorder::Application(spec);
    app.Run();
}

