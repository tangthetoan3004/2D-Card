#include "MoveCommand.h"

MoveCommand::MoveCommand(const std::vector<Vertex*>& vertices, float dx, float dy)
    : mVertices(vertices), mDx(dx), mDy(dy)
{
}

void MoveCommand::Execute()
{
    for (auto* v : mVertices)
    {
        if (v) v->Move(mDx, mDy);
    }
}

void MoveCommand::Undo()
{
    for (auto* v : mVertices)
    {
        if (v) v->Move(-mDx, -mDy);
    }
}
