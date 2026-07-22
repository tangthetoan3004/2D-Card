#include "DrawFaceCommand.h"

DrawFaceCommand::DrawFaceCommand(Scene* scene, Face* face, const std::vector<Vertex*>& vertices)
    : mScene(scene), mFace(face), mVertices(vertices)
{
}

void DrawFaceCommand::Execute()
{
    if (!mScene) return;
    std::list<Shape*> shapes = mScene->retShapes();
    for (auto* v : mVertices)
    {
        shapes.push_back(v);
    }
    if (mFace) shapes.push_back(mFace);
    mScene->UpdateShapes(shapes);
}

void DrawFaceCommand::Undo()
{
    if (!mScene) return;
    std::list<Shape*> shapes = mScene->retShapes();
    if (mFace) shapes.remove(mFace);
    for (auto* v : mVertices)
    {
        shapes.remove(v);
    }
    mScene->UpdateShapes(shapes);
}
