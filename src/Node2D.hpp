#pragma once
#include "Core.hpp"
#include "Node.hpp"
#include "Math.hpp"


class API_EXPORT Node2D : public Node
{
public:
    Node2D();
    virtual ~Node2D();
    Node2D(const std::string &name);



    Matrix2D getTransform() ;
    Matrix2D getWorldTransform() ;

    virtual void OnUpdate(double deltaTime) override;
    virtual void OnDraw() override;

    void setPosition(float x, float y);
    void setPosition(Vector2 position);

    void move(float x, float y);
    void move(Vector2 position);

    void advance(float speed);
    void advance(float speed, float angle);

    void setScale(float x, float y);
    void setScale(Vector2 scale);



    Vector2 position;
    Vector2 scale;
    Vector2 origin;
    Vector2 skew;
    float rotation;
    Matrix2D transform;
    Matrix2D local_transform;
    Matrix2D wordl_transform;
    Node2D *node_parent{nullptr};
};
 