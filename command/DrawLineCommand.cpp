#include "DrawLineCommand.h"

DrawLineCommand::DrawLineCommand(Scene* scene, Line* line, const std::vector<Vertex*>& vertices)
    : mScene(scene), mLine(line), mVertices(vertices)
{
}

void DrawLineCommand::Execute()
{
    if (!mScene) return;
    std::list<Shape*> shapes = mScene->retShapes();
    for (auto* v : mVertices)
    {
        shapes.push_back(v);
    }
    if (mLine) shapes.push_back(mLine);
    mScene->UpdateShapes(shapes);
}

void DrawLineCommand::Undo()
{
    if (!mScene) return;
    std::list<Shape*> shapes = mScene->retShapes();
    if (mLine) shapes.remove(mLine);
    for (auto* v : mVertices)
    {
        shapes.remove(v);
    }
    mScene->UpdateShapes(shapes);
}
