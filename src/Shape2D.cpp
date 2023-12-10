#include "Shape2D.hpp"
#include "Node2D.hpp"



bool can_collide(Shape2D* a, Shape2D* b)
{
    if (!a || !b) return false;

    if ((a->group & b->mask) != 0 &&
        (b->group & a->mask) != 0)
    {
        return true; 
    }

    return false; 
}

Shape2D::Shape2D()
{
   
    this->type  = SHAPE2D;

    world = {0.0f, 0.0f};
    offset.x = 0.0f;
    offset.y = 0.0f;
    show = true;
    group = 0;
    mask = 0;
   
}

Shape2D::~Shape2D()
{
}
void Shape2D::setVisible(bool visible)
{
    this->show = visible;
}

bool Shape2D::collide(float x, float y, Shape2D *other)
{
  
    if (this->type==CIRCLESHAPE2D)
    {
        CircleShape2D *circle = static_cast<CircleShape2D *>(this);
        if (other->type==CIRCLESHAPE2D)
        {
            CircleShape2D *circle2 = static_cast<CircleShape2D *>(other);
            return circle->collide(x,y,circle2);
        } else if (other->type==RECTANGLESHAPE2D)
        {
            RectangleShape2D *rect = static_cast<RectangleShape2D *>(other);
            return rect->collide(x,y,circle);
        } else if (other->type==LISTSHAPE2D)
        {
             ListShape2D *list = static_cast<ListShape2D *>(other);
            for (int i=0;i<list->count();i++)
            {
                Shape2D *shape = list->get(i);
                if (collide(x,y,shape)) return true;
            }            
        }
    } else if (this->type==RECTANGLESHAPE2D)
    {
        RectangleShape2D *rect = static_cast<RectangleShape2D *>(this);
        if (other->type==CIRCLESHAPE2D)
        {
            CircleShape2D *circle = static_cast<CircleShape2D *>(other);
            return rect->collide(x,y,circle);
        } else if (other->type==RECTANGLESHAPE2D)
        {
            RectangleShape2D *rect2 = static_cast<RectangleShape2D *>(other);
            return rect->collide(x,y,rect2);
        } else if (other->type==LISTSHAPE2D)
        {
            ListShape2D *list = static_cast<ListShape2D *>(other);
            for (int i=0;i<list->count();i++)
            {
                Shape2D *shape = list->get(i);
                if (collide(x,y,shape)) return true;
            }
        }
    } else 
    if (this->type==LISTSHAPE2D)
    {
        ListShape2D *list = static_cast<ListShape2D *>(this);
        for (int i=0;i<list->count();i++)
        {
            Shape2D *shape = list->get(i);
           if (shape->collide(x,y,other))
                return true;
        }
    }
    return false;  
}



bool Shape2D::collide(Shape2D *other)
{

    if (this->type==CIRCLESHAPE2D)
    {
        CircleShape2D *circle = static_cast<CircleShape2D *>(this);
        if (other->type==CIRCLESHAPE2D)
        {
            CircleShape2D *circle2 = static_cast<CircleShape2D *>(other);
            return circle->collide(circle2);
        } else if (other->type==RECTANGLESHAPE2D)
        {
            RectangleShape2D *rect = static_cast<RectangleShape2D *>(other);
            return rect->collide(circle);
        } else if (other->type==LISTSHAPE2D)
        {
            ListShape2D *list = static_cast<ListShape2D *>(other);
            for (int i=0;i<list->count();i++)
            {
                Shape2D *shape = list->get(i);
                if (shape->type==CIRCLESHAPE2D)
                {
                    CircleShape2D *circle2 = static_cast<CircleShape2D *>(shape);
                    if (circle->collide(circle2))
                        return true;
                } else if (shape->type==RECTANGLESHAPE2D)
                {
                    RectangleShape2D *rect = static_cast<RectangleShape2D *>(shape);
                    if (rect->collide(circle))
                        return true;
                }
               
            }
        }
    } else if (this->type==RECTANGLESHAPE2D)
    {
        RectangleShape2D *rect = static_cast<RectangleShape2D *>(this);
        if (other->type==CIRCLESHAPE2D)
        {
            CircleShape2D *circle = static_cast<CircleShape2D *>(other);
            return rect->collide(circle);
        } else if (other->type==RECTANGLESHAPE2D)
        {
            RectangleShape2D *rect2 = static_cast<RectangleShape2D *>(other);
            return rect->collide(rect2);
        } else if (other->type==LISTSHAPE2D)
        {
            ListShape2D *list = static_cast<ListShape2D *>(other);
            for (int i=0;i<list->count();i++)
            {
                Shape2D *shape = list->get(i);
                if (shape->type==CIRCLESHAPE2D)
                {
                    CircleShape2D *circle2 = static_cast<CircleShape2D *>(shape);
                    if (rect->collide(circle2))
                        return true;
                } else if (shape->type==RECTANGLESHAPE2D)
                {
                    RectangleShape2D *rect2 = static_cast<RectangleShape2D *>(shape);
                   
                    if (rect->collide(rect2))
                        return true;

                    
                }
               
            }
        }
    } else 
    if (this->type==LISTSHAPE2D)
    {
        ListShape2D *list = static_cast<ListShape2D *>(this);
        for (int i=0;i<list->count();i++)
        {
            Shape2D *child = list->get(i);
          //  if (child->collide(other))    return true;    
        }
    }

    return false;
}



void Shape2D::setMask(long mask)
{
    this->mask = mask;
}

void Shape2D::setGroup(long group)
{
    this->group = group;
}

void Shape2D::setGroups(long group, long mask)
{
    this->group = group;
    this->mask = mask;
}

bool Shape2D::can(Shape2D *shape)
{
    // if (!shape) return false;

    // if ((group & shape->mask) != 0 && (shape->group & mask) != 0)
    // {
    //     return true; 
    // }
    return true;
}


CircleShape2D::CircleShape2D(float radius):Shape2D()
{

   
    this->type  = CIRCLESHAPE2D;
    this->radius = radius;
    this->center.x = 0.0f;
    this->center.y = 0.0f;

}

CircleShape2D::CircleShape2D(float radius, Vector2 center):Shape2D()
{

    this->type  = CIRCLESHAPE2D;
    this->radius = radius;
    this->center.x = center.x;
    this->center.y = center.y;
}

CircleShape2D::CircleShape2D(float radius, float x, float y):Shape2D()
{

    this->type  = CIRCLESHAPE2D;
    this->radius = radius;
    this->center.x = x;
    this->center.y = y;
}

void CircleShape2D::transform(Matrix2D *transform)
{
    if (!manual)
    {
        world = transform->TransformCoords(center);
    }
}

void CircleShape2D::render()
{
 
    if (this->show)
        DrawCircleLines(world.x,world.y, radius, LIME);
    
}

void CircleShape2D::update()
{
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

bool CircleShape2D::collide(float x, float y, CircleShape2D *circle)
{
    this->center.x = x;
    this->center.y = y;
    return CheckCollisionCircles(this->center, radius, circle->world, circle->radius);
}


RectangleShape2D::RectangleShape2D(float width, float height):Shape2D()
{
   
    this->type  = RECTANGLESHAPE2D;
    this->width = width;
    this->height = height;
    this->center = {0.0f, 0.0f};
    this->world = {0.0f, 0.0f};
}


RectangleShape2D::RectangleShape2D(float width, float height, Vector2 center):Shape2D()
{

    this->type  = RECTANGLESHAPE2D;
    this->width = width;
    this->height = height;
    this->center.x = center.x;
    this->center.y = center.y;
    this->world.x = center.x;
    this->world.y = center.y;
}

RectangleShape2D::RectangleShape2D(float width, float height, float x, float y):Shape2D()
{
  
    this->type  = RECTANGLESHAPE2D;
    this->width = width;
    this->height = height;
    this->center.x = x;
    this->center.y = y;
    this->world.x = x;
    this->world.y = y;
    
}

void RectangleShape2D::render()
{
    
    if (this->show)
    {
        if (manual)
        {
            DrawRectangle(rectangle.x, rectangle.y, rectangle.width, rectangle.height, Fade(LIME,0.5));
         
        } else 
        {
           DrawRectangle(world.x,world.y, width, height, Fade(LIME,0.5));
        }
    }

}

void RectangleShape2D::update()
{
    if (manual) return;
    rectangle.x = world.x;
    rectangle.y = world.y;
    rectangle.width = width;
    rectangle.height = height;
}

void RectangleShape2D::transform(Matrix2D *transform)
{
    if (manual) return;
    world = transform->TransformCoords(this->center);
  //  world.x -= width/2;
  //  world.y -= height/2;
}

bool RectangleShape2D::collide(Vector2 point)
{
    update();
    return CheckCollisionPointRec(point,rectangle);
}

bool RectangleShape2D::collide(float x, float y)
{
    update();
    return CheckCollisionPointRec({x,y},rectangle);
}

bool RectangleShape2D::collide(RectangleShape2D *shape)
{
    update();
    shape->update();
    return CheckCollisionRecs(rectangle, shape->rectangle);
}

bool RectangleShape2D::collide(CircleShape2D *circle)
{

    return CheckCollisionCircleRec(circle->world, circle->radius, rectangle);
}

bool RectangleShape2D::collide(float x, float y, RectangleShape2D *shape)
{
    manual = true;
    rectangle.x =   x - ((width + offset.x )   *0.5f ) ;
    rectangle.y =   y - ((height+ offset.y)   *0.5f ) ;
    rectangle.width  = width;
    rectangle.height = height;
    shape->update();
    return  CheckCollisionRecs(rectangle, shape->rectangle);
}

bool RectangleShape2D::collide(float x, float y, CircleShape2D *circle)
{
    manual = true;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.width = width;
    rectangle.height = height;

    return CheckCollisionCircleRec(circle->world, circle->radius, rectangle);
    
}

bool RectangleShape2D::collide(float x, float y, float radius)
{
    update();
    return CheckCollisionCircleRec({x,y}, radius, rectangle);
}

bool RectangleShape2D::collide(float x, float y, float w, float h)
{
    update();
    return CheckCollisionRecs(rectangle, {x,y,w,h});
}

PolyShape2D::PolyShape2D():Shape2D()
{
   
    this->type  = POLYSHAPE2D;
}

PolyShape2D::PolyShape2D(std::vector<Vector2> points)
{
   
    this->type  = POLYSHAPE2D;
    this->points = points;
    world.reserve(points.size());
}

void PolyShape2D::render()
{
  
    if (this->show)
    {
        for (int i=0;i<(int)points.size()-1;i++)
        {
            DrawLineEx(world[i], world[i+1], 1, LIME);
        }
        DrawLineEx(world[0], world[world.size()-1], 1, LIME);
    }

}

void PolyShape2D::update()
{
    world.clear();
    world.reserve(points.size());
    for (int i=0;i<(int)points.size();i++)
    {
        world.push_back(points[i]);
    }
}

void PolyShape2D::transform(Matrix2D *transform)
{
    for (int i=0;i<(int)points.size();i++)
    {
        world[i] = transform->TransformCoords(points[i]);
    }
}


ListShape2D::ListShape2D()
{
    this->type  = LISTSHAPE2D;
}
ListShape2D::~ListShape2D()
{
    for (int i=0;i<(int)shapes.size();i++)
    {
        delete shapes[i];
    }
    shapes.clear();
}

void ListShape2D::update()
{
    for (int i=0;i<(int)shapes.size();i++)
    {
        shapes[i]->update();
    }
}

void ListShape2D::transform(Matrix2D *transform)
{
    for (int i=0;i<(int)shapes.size();i++)
    {
        shapes[i]->transform(transform);
    }
}

void ListShape2D::render()
{
    for (int i=0;i<(int)shapes.size();i++)
    {
        shapes[i]->render();
    }
}

int ListShape2D::count()
{
    return(int) shapes.size();
}

Shape2D *ListShape2D::add(Shape2D *shape)
{
    shape->update();
    shapes.push_back(shape);
    return shape;
}



void ListShape2D::clear()
{
    for (int i=0;i<(int)shapes.size();i++)
    {
        delete shapes[i];
    }
    shapes.clear();
}

Shape2D *ListShape2D::get(int index)
{
    if (index < 0 || index >= count()) return nullptr;
    return shapes[index];
}