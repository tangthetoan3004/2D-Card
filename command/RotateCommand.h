#pragma once
#include "Command.h"
#include "../shape/Shape.h"
#include <vector>
#include <QPointF>

class RotateCommand : public Command
{
public:
    RotateCommand(const std::vector<Vertex*>& vertices, const QPointF& pivot, float angleRad);
    ~RotateCommand() override = default;

    void Execute() override;
    void Undo() override;

private:
    std::vector<Vertex*> mVertices;
    QPointF mPivot;
    float mAngleRad;
};
