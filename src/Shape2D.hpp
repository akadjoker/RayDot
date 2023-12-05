#pragma once
#include "Core.hpp"
#include "Node.hpp"
#include "Node2D.hpp"

class API_EXPORT Shape2D : public Node
{
    public:
    Shape2D();
    virtual ~Shape2D();
    virtual void OnReady() override;
    bool collide(Shape2D *shape);
    void setVisible(bool visible);
    protected:
        Node2D *nparent;
        Vector2 world;
        bool show{false};

};


class API_EXPORT CircleShape2D : public Shape2D
{
    public:
        friend class Node2D;
        friend class RectangleShape2D;

        CircleShape2D();
        CircleShape2D(float radius);
        CircleShape2D(float radius, Vector2 center);
        CircleShape2D(float radius, float x, float y);

        virtual void OnUpdate(double deltaTime) override;
        virtual void OnDraw() override;

        bool collide(Vector2 point);
        bool collide(float x, float y);
        bool collide(float x, float y, float radius);
        bool collide(CircleShape2D *circle);
        bool collide(float x, float y, float w, float h);
        


        float   radius;
        Vector2 center;
        


    

};

class API_EXPORT RectangleShape2D : public Shape2D
{
    public:
        RectangleShape2D();
        RectangleShape2D(float width, float height);
        RectangleShape2D(float width, float height, Vector2 center);
        RectangleShape2D(float width, float height, float x, float y);
        virtual void OnDraw() override;
        virtual void OnUpdate(double deltaTime) override;

        bool collide(Vector2 point);
        bool collide(float x, float y);
        bool collide(RectangleShape2D *shape);
        bool collide(CircleShape2D *circle);
        bool collide(float x, float y, float radius);
        bool collide(float x, float y, float w, float h);


        float   width;
        float   height;
        Vector2 center;

    private:
        Rectangle rectangle;
};


class API_EXPORT PolyShape2D : public Shape2D
{
    public:
        PolyShape2D();
        PolyShape2D(std::vector<Vector2> points);
        virtual void OnDraw() override;
        virtual void OnUpdate(double deltaTime) override;
    protected:
        std::vector<Vector2> points;
        std::vector<Vector2> world;

};