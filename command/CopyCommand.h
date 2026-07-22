#pragma once
#include "Command.h"
#include "../viewport/Scene.h"
#include "../shape/Shape.h"
#include <vector>

class CopyCommand : public Command
{
public:
    CopyCommand(Scene* scene, const std::vector<Shape*>& newShapes, const std::vector<Vertex*>& newVertices);
    ~CopyCommand() override;

    void Execute() override;
    void Undo() override;

private:
    Scene* mScene;
    std::vector<Shape*> mNewShapes;
    std::vector<Vertex*> mNewVertices;
    bool mIsInScene;
};
