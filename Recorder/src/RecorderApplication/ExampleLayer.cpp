//
// Created by Kasper de Bruin on 22-10-2023.
//

#include "ExampleLayer.h"

ExampleLayer::ExampleLayer()
: Layer("Example"), videoReader(nullptr)
{
    CL_WARN("ExampleLayer::ExampleLayer constructor");
}

void ExampleLayer::OnAttach() {
    CL_WARN("ExampleLayer::OnAttach");

    videoReader = VideoReader::Create(
        VideoProps(
            "D:\\OPENCV\\Assets\\Left.mp4",
            1280,
            720
        )
    );    

}

void ExampleLayer::OnUpdate() {
    videoReader->Read();
    videoReader->Show();
}

void ExampleLayer::OnDetach() {
    CL_WARN("ExampleLayer::OnDetach");
}