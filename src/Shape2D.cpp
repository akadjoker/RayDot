#include "Shape2D.hpp"
#include "Node2D.hpp"


Shape2D::Shape2D():Node()
{
    this->name = "Shape2D";
    this->type  = SHAPE2D;
    nparent = nullptr;
    world = {0.0f, 0.0f};
}

Shape2D::~Shape2D()
{
}
void Shape2D::setVisible(bool visible)
{
    this->show = visible;
}
void Shape2D::OnReady()
{
    Node::OnReady();
    if (parent)
    {
       nparent = dynamic_cast<Node2D *>(this->parent);
    }
}

bool Shape2D::collide(Shape2D *shape)
{
    if (this->type==CIRCLESHAPE2D)
    {
        CircleShape2D *circle = static_cast<CircleShape2D *>(this);
        if (shape->type==CIRCLESHAPE2D)
        {
            CircleShape2D *circle2 = static_cast<CircleShape2D *>(shape);
            return circle->collide(circle2);
        } else if (shape->type==RECTANGLESHAPE2D)
        {
            RectangleShape2D *rect = static_cast<RectangleShape2D *>(shape);
            return rect->collide(circle);
        }
    } else if (this->type==RECTANGLESHAPE2D)
    {
        RectangleShape2D *rect = static_cast<RectangleShape2D *>(this);
        if (shape->type==CIRCLESHAPE2D)
        {
            CircleShape2D *circle = static_cast<CircleShape2D *>(shape);
            return rect->collide(circle);
        } else if (shape->type==RECTANGLESHAPE2D)
        {
            RectangleShape2D *rect2 = static_cast<RectangleShape2D *>(shape);
            return rect->collide(rect2);
        }
    }
    return false;
}



CircleShape2D::CircleShape2D():Shape2D()
{
    this->name = "CircleShape2D";
    this->type  = CIRCLESHAPE2D;
    this->radius = 0.0f;
    this->center = {0.0f, 0.0f};
}

CircleShape2D::CircleShape2D(float radius):Shape2D()
{

    this->name = "CircleShape2D";
    this->type  = CIRCLESHAPE2D;
    this->radius = radius;
    this->center.x = 0.0f;
    this->center.y = 0.0f;

}

CircleShape2D::CircleShape2D(float radius, Vector2 center):Shape2D()
{
    this->name = "CircleShape2D";
    this->type  = CIRCLESHAPE2D;
    this->radius = radius;
    this->center.x = center.x;
    this->center.y = center.y;
}

CircleShape2D::CircleShape2D(float radius, float x, float y):Shape2D()
{
    this->name = "CircleShape2D";
    this->type  = CIRCLESHAPE2D;
    this->radius = radius;
    this->center.x = x;
    this->center.y = y;
}
void CircleShape2D::OnUpdate(double deltaTime)
{
    Node::OnUpdate(deltaTime);
    if (nparent)
    {
        world = nparent->transform.TransformCoords(this->center);
    } else 
    {
        world.x = center.x;
        world.y = center.y;
    }
}

void CircleShape2D::OnDraw()
{
    Node::OnDraw();
    if (this->show)
        DrawCircleLines(world.x,world.y, radius, LIME);
    
}

bool CircleShape2D::collide(Vector2 point)
{
    return CheckCollisionPointCircle(point, world, radius);
}

bool CircleShape2D::collide(float x, float y)
{
    return CheckCollisionPointCircle({x,y}, world, radius);
}

bool CircleShape2D::collide(float x, float y, float radius)
{
    return CheckCollisionCircles(world, this->radius, {x,y}, radius);
}

bool CircleShape2D::collide(CircleShape2D *circle)
{
    return CheckCollisionCircles(world, radius, circle->world, circle->radius);
}

bool CircleShape2D::collide(float x, float y, float w, float h)
{
    return CheckCollisionCircleRec(world, radius, {x,y,w,h});
}

RectangleShape2D::RectangleShape2D():Shape2D()
{
    this->name = "RectangleShape2D";
    this->type  = RECTANGLESHAPE2D;
    this->width = 0.0f;
    this->height = 0.0f;
    this->center = {0.0f, 0.0f};
}

RectangleShape2D::RectangleShape2D(float width, float height):Shape2D()
{
    this->name = "RectangleShape2D";
    this->type  = RECTANGLESHAPE2D;
    this->width = width;
    this->height = height;
    this->center = {0.0f, 0.0f};
}


RectangleShape2D::RectangleShape2D(float width, float height, Vector2 center):Shape2D()
{
    this->name = "RectangleShape2D";
    this->type  = RECTANGLESHAPE2D;
    this->width = width;
    this->height = height;
    this->center.x = center.x;
    this->center.y = center.y;
}

RectangleShape2D::RectangleShape2D(float width, float height, float x, float y):Shape2D()
{
    this->name = "RectangleShape2D";
    this->type  = RECTANGLESHAPE2D;
    this->width = width;
    this->height = height;
    this->center.x = x;
    this->center.y = y;
}

void RectangleShape2D::OnDraw()
{
    Node::OnDraw();
    if (this->show)
        DrawRectangleLines(world.x,world.y, width, height, LIME);
}

void RectangleShape2D::OnUpdate(double deltaTime)
{
    Node::OnUpdate(deltaTime);
    if (nparent)
    {
        world = nparent->transform.TransformCoords(this->center);
        world.x -= width/2;
        world.y -= height/2;
    } else 
    {
        world.x = center.x;//x - width/2;
        world.y = center.y;// - height/2;
    }
    rectangle.x = world.x;
    rectangle.y = world.y;
    rectangle.width = width;
    rectangle.height = height;
}

bool RectangleShape2D::collide(Vector2 point)
{
    return CheckCollisionPointRec(point,rectangle);
}

bool RectangleShape2D::collide(float x, float y)
{
    return CheckCollisionPointRec({x,y},rectangle);
}

bool RectangleShape2D::collide(RectangleShape2D *shape)
{
    return CheckCollisionRecs(rectangle, shape->rectangle);
}

bool RectangleShape2D::collide(CircleShape2D *circle)
{
    return CheckCollisionCircleRec(circle->world, circle->radius, rectangle);
}

bool RectangleShape2D::collide(float x, float y, float radius)
{
    return CheckCollisionCircleRec({x,y}, radius, rectangle);
}

bool RectangleShape2D::collide(float x, float y, float w, float h)
{
    return CheckCollisionRecs(rectangle, {x,y,w,h});
}

PolyShape2D::PolyShape2D():Shape2D()
{
    this->name = "PolyShape2D";
    this->type  = POLYSHAPE2D;
}

PolyShape2D::PolyShape2D(std::vector<Vector2> points)
{
    this->name = "PolyShape2D";
    this->type  = POLYSHAPE2D;
    this->points = points;
    world.reserve(points.size());
}

void PolyShape2D::OnDraw()
{
    Node::OnDraw();
    if (this->show)
    {
        for (int i=0;i<(int)points.size()-1;i++)
        {
            DrawLineEx(world[i], world[i+1], 1, LIME);
        }
        DrawLineEx(world[0], world[world.size()-1], 1, LIME);
    }

}

void PolyShape2D::OnUpdate(double deltaTime)
{
    Node::OnUpdate(deltaTime);
    if (nparent)
    {
        for (int i=0;i<(int)points.size();i++)
        {
            world[i] = nparent->transform.TransformCoords(points[i]);
        }
    }
}
