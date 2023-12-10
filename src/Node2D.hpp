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
    

    void setPosition(float x, float y);
    void setPosition(Vector2 position);

    void move(float x, float y);
    void move(Vector2 position);

    void moveBy(float x, float y, Node *node, bool sweep = false);
        
	Vector2 getWorldPosition(const Vector2 &v) ;
    Vector2 getWorldPosition(float x, float y) ;


    void advance(float speed);
    void advance(float speed, float angle);

    void setScale(float x, float y);
    void setScale(Vector2 scale);

    float getWorldRotation() const;
    float getWorldX() const;
    float getWorldY() const;
    float getWorldScaleX() const;
    float getWorldScaleY() const;
    float getWorldOriginX() const;
    float getWorldOriginY() const;
    float getRealX() const;
    float getRealY() const;
    float getX() const; 
    float getY() const;
    

    Vector2 position;
    Vector2 scale;
    Vector2 origin;
    Vector2 skew;
    float rotation;
    Matrix2D transform;
    Matrix2D local_transform;
    Matrix2D wordl_transform;
    protected:
    virtual void updateBound() override;
    float _moveX{0.0f};
    float _moveY{0.0f};
    float _x{0.0f};
    float _y{0.0f};
    
};
 