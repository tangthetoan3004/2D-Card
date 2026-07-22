#pragma once
#include "Command.h"
#include "../viewport/Scene.h"
#include "../shape/Shape.h"
#include <vector>

class DrawFaceCommand : public Command
{
public:
    DrawFaceCommand(Scene* scene, Face* face, const std::vector<Vertex*>& vertices);
    ~DrawFaceCommand() override = default;

    void Execute() override;
    void Undo() override;

private:
    Scene* mScene;
    Face* mFace;
    std::vector<Vertex*> mVertices;
};
