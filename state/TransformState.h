#pragma once
#include "State.h"
#include "../shape/Shape.h"
#include <vector>

enum class TransformMode
{
    MOVE,
    COPY,
    ROTATE,
    SCALE
};

class TransformState : public State
{
public:
    TransformState(const std::string& name, SelectUtils::ViewportData* data, TransformMode mode = TransformMode::MOVE);

    void SetMode(TransformMode mode);
    virtual void UpdateScene(Scene* scene) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPainter* painter) override;

private:
    std::string mName;
    Viewport* mViewport;
    Scene* mScene;
    Camera* mCamera;

    TransformMode mMode;
    QPointF mPos;
    QPointF mBasePoint;
    bool mHasBasePoint;
    int mButton;
};
