#pragma once
#include "Command.h"
#include "../viewport/Scene.h"
#include "../shape/Shape.h"
#include <vector>

class DrawLineCommand : public Command
{
public:
    DrawLineCommand(Scene* scene, Line* line, const std::vector<Vertex*>& vertices);
    ~DrawLineCommand() override = default;

    void Execute() override;
    void Undo() override;

private:
    Scene* mScene;
    Line* mLine;
    std::vector<Vertex*> mVertices;
};
