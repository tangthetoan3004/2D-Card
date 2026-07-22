#pragma once
#include "Command.h"
#include "../shape/Shape.h"
#include <vector>
#include <QPointF>

class ScaleCommand : public Command
{
public:
    ScaleCommand(const std::vector<Vertex*>& vertices, const QPointF& pivot, float scaleFactor);
    ~ScaleCommand() override = default;

    void Execute() override;
    void Undo() override;

private:
    std::vector<Vertex*> mVertices;
    QPointF mPivot;
    float mScaleFactor;
};
