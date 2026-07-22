#include "DeleteShapeCommand.h"

DeleteShapeCommand::DeleteShapeCommand(Scene* scene, const std::vector<Shape*>& targetShapes)
    : mScene(scene), mDeletedShapes(targetShapes), mIsDeletedFromScene(false)
{
}

DeleteShapeCommand::~DeleteShapeCommand()
{
    // Nếu các hình đang nằm ngoài Scene (đã bị xóa khỏi Scene) khi đối tượng Command này bị hủy,
    // thực hiện giải phóng bộ nhớ cho từng con trỏ Shape
    if (mIsDeletedFromScene)
    {
        for (Shape* s : mDeletedShapes)
        {
            delete s;
        }
        mDeletedShapes.clear();
    }
}

void DeleteShapeCommand::Execute()
{
    if (!mScene || mDeletedShapes.empty()) return;

    std::list<Shape*> shapes = mScene->retShapes();
    for (Shape* s : mDeletedShapes)
    {
        shapes.remove(s);
    }
    mScene->UpdateShapes(shapes);
    mIsDeletedFromScene = true;
}

void DeleteShapeCommand::Undo()
{
    if (!mScene || mDeletedShapes.empty()) return;

    std::list<Shape*> shapes = mScene->retShapes();
    for (Shape* s : mDeletedShapes)
    {
        shapes.push_back(s);
    }
    mScene->UpdateShapes(shapes);
    mIsDeletedFromScene = false;
}
