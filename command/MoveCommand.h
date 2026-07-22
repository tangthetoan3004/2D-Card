#pragma once
#include "Command.h"
#include "../shape/Shape.h"
#include <vector>

class MoveCommand : public Command
{
public:
    MoveCommand(const std::vector<Vertex*>& vertices, float dx, float dy);
    ~MoveCommand() override = default;

    void Execute() override;
    void Undo() override;

private:
    std::vector<Vertex*> mVertices;
    float mDx, mDy;
};
