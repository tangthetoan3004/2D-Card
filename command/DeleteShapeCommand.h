#pragma once
#include "Command.h"
#include "../viewport/Scene.h"
#include "../shape/Shape.h"
#include <vector>

class DeleteShapeCommand : public Command
{
public:
    DeleteShapeCommand(Scene* scene, const std::vector<Shape*>& targetShapes);
    ~DeleteShapeCommand() override;

    void Execute() override;
    void Undo() override;

private:
    Scene* mScene;
    std::vector<Shape*> mDeletedShapes;
    bool mIsDeletedFromScene;
};
