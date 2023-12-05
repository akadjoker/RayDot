#include "Node2D.hpp"
#include <raymath.h>

Node2D::Node2D():Node()
{
    this->name = "Node2D";
    this->type  = NODE2D;
    this->position = {0.0f, 0.0f};
    this->scale = {1.0f, 1.0f};
    this->origin= {0.0f, 0.0f};
    this->rotation = 0.0f;
    this->skew = {0.0f, 0.0f};
    
    this->transform.Identity();

}

Node2D::~Node2D()
{
}

Node2D::Node2D(const std::string &name):Node(name)
{
    this->name = name;
    this->type  = NODE2D;
    this->position = {0.0f, 0.0f};
    this->scale = {1.0f, 1.0f};
    this->origin= {0.0f, 0.0f};
    this->rotation = 0.0f;
    this->skew = {0.0f, 0.0f};
    
    this->transform.Identity();
}

void Node2D::OnUpdate(double deltaTime) 
{
    Node::OnUpdate(deltaTime);
    
    
}

void Node2D::OnDraw()
{
    Node::OnDraw();
  //  DrawCircleLines(position.x, position.y, 5, RED);
}

void Node2D::setPosition(float x, float y)
{
    this->position.x = x;
    this->position.y = y;
}

void Node2D::setPosition(Vector2 position)
{
    this->position = position;

}

void Node2D::move(float x, float y)
{
    this->position.x += x;
    this->position.y += y;
}

void Node2D::move(Vector2 position)
{
    this->position.x += position.x;
    this->position.y += position.y;
}

void Node2D::advance(float speed)
{
    this->position.x += cos(rotation * RAD) * speed;
    this->position.y += sin(rotation * RAD) * speed;
}

void Node2D::advance(float speed, float angle)
{
    this->position.x += cos(angle * RAD) * speed;
    this->position.y += sin(angle * RAD) * speed;
}

void Node2D::setScale(float x, float y)
{
    this->scale.x = x;
    this->scale.y = y;
}

void Node2D::setScale(Vector2 scale)
{
    this->scale = scale;
    
}

Matrix2D Matrix2DMult(const Matrix2D curr, const Matrix2D m)
{

    Matrix2D result;

    result.a = curr.a * m.a + curr.b * m.c;
    result.b = curr.a * m.b + curr.b * m.d;
    result.c = curr.c * m.a + curr.d * m.c;
    result.d = curr.c * m.b + curr.d * m.d;

    result.tx = curr.tx * m.a + curr.ty * m.c + m.tx;
    result.ty = curr.tx * m.b + curr.ty * m.d + m.ty;

    return result;
}




Matrix2D Node2D::getTransform() 
{
    transform.Identity();
    if (skew.x == 0.0f && skew.y == 0.0f)
    {

        if (rotation == 0.0)
        {

            transform.Set(scale.x, 0.0, 0.0, scale.y, position.x - origin.x * scale.x, position.y - origin.y * scale.y);
        }
        else
        {
            float acos = cos(rotation * RAD);
            float asin = sin(rotation * RAD);
            float a = scale.x * acos;
            float b = scale.x * asin;
            float c = scale.y * -asin;
            float d = scale.y * acos;
            float tx = position.x - origin.x * a - origin.y * c;
            float ty = position.y - origin.x * b - origin.y * d;

            transform.Set(a, b, c, d, tx, ty);
        }
    }
    else
    {

        transform.Identity();
        transform.Scale(scale.x, scale.y);
        transform.Skew(skew.x, skew.y);
        transform.Rotate(rotation);
        transform.Translate(position.x, position.y);

        if (origin.x != 0.0f || origin.y != 0.0f)
        {

            transform.tx = position.x - transform.a * origin.x - transform.c * origin.y;
            transform.ty = position.y - transform.b * origin.x - transform.d * origin.y;
        }
    }

    return transform;


}

Matrix2D Node2D::getWorldTransform() 
{
    local_transform = getTransform();
    if (parent != nullptr)
    {
        if (parent->getType() == NODE2D || parent->getType() == SPRITE2D)
        {
            if (node_parent == nullptr)
             node_parent = dynamic_cast<Node2D *>(this->parent);
        }
        
        if (node_parent!=nullptr)
        {
            Matrix2D mat = node_parent->getWorldTransform();
            wordl_transform = Matrix2DMult(local_transform, mat);
            return wordl_transform;
        } else
        {
            return local_transform;
        }
    }
    return local_transform;

   
}
