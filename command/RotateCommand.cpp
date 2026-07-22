#include "RotateCommand.h"

RotateCommand::RotateCommand(const std::vector<Vertex*>& vertices, const QPointF& pivot, float angleRad)
    : mVertices(vertices), mPivot(pivot), mAngleRad(angleRad)
{
}

void RotateCommand::Execute()
{
    for (auto* v : mVertices)
    {
        if (v) v->Rotate(mPivot, mAngleRad);
    }
}

void RotateCommand::Undo()
{
    for (auto* v : mVertices)
    {
        if (v) v->Rotate(mPivot, -mAngleRad);
    }
}
