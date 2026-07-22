#include "ScaleCommand.h"

ScaleCommand::ScaleCommand(const std::vector<Vertex*>& vertices, const QPointF& pivot, float scaleFactor)
    : mVertices(vertices), mPivot(pivot), mScaleFactor(scaleFactor)
{
}

void ScaleCommand::Execute()
{
    if (mScaleFactor == 0.0f) return;
    for (auto* v : mVertices)
    {
        if (v) v->Scale(mPivot, mScaleFactor);
    }
}

void ScaleCommand::Undo()
{
    if (mScaleFactor == 0.0f) return;
    for (auto* v : mVertices)
    {
        if (v) v->Scale(mPivot, 1.0f / mScaleFactor);
    }
}
