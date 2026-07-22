#include "CopyCommand.h"

CopyCommand::CopyCommand(Scene* scene, const std::vector<Shape*>& newShapes, const std::vector<Vertex*>& newVertices)
    : mScene(scene), mNewShapes(newShapes), mNewVertices(newVertices), mIsInScene(false)
{
}

CopyCommand::~CopyCommand()
{
    if (!mIsInScene)
    {
        for (auto* s : mNewShapes)
        {
            delete s;
        }
        for (auto* v : mNewVertices)
        {
            delete v;
        }
    }
}

void CopyCommand::Execute()
{
    if (!mScene) return;

    std::list<Shape*> shapes = mScene->retShapes();
    for (auto* v : mNewVertices)
    {
        shapes.push_back(v);
    }
    for (auto* s : mNewShapes)
    {
        shapes.push_back(s);
    }
    mScene->UpdateShapes(shapes);
    mIsInScene = true;
}

void CopyCommand::Undo()
{
    if (!mScene) return;

    std::list<Shape*> shapes = mScene->retShapes();
    for (auto* s : mNewShapes)
    {
        shapes.remove(s);
    }
    for (auto* v : mNewVertices)
    {
        shapes.remove(v);
    }
    mScene->UpdateShapes(shapes);
    mIsInScene = false;
}
