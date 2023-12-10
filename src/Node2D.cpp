#include "Node2D.hpp"
#include "Shape2D.hpp"
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
    this->transform = getWorldTransform();
    updateBound();

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
    this->transform = getWorldTransform();
    updateBound();
}

void Node2D::updateBound()
{
    AABB::Transform(position, origin, scale, rotation,width,height,&bound);
    if (shape!=nullptr)
    {
       shape->transform(&transform);
    }
}

void Node2D::OnUpdate(double deltaTime) 
{
    Node::OnUpdate(deltaTime);
    this->transform = getWorldTransform();
    updateBound();
    
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

void Node2D::moveBy(float x, float y, Node *node, bool sweep)
{
    _moveX += x;
    _moveY += y;
    x = round(_moveX);
    y = round(_moveY);
    _moveX -= x;
    _moveY -= y;
    
        int sign;
        if (node!=nullptr)
        {
            
            if (x != 0)
            {
                if (sweep || Collide(this->position.x + x, this->position.y,node))
                {
                    sign = x > 0 ? 1 : -1;
                    while (x != 0)
                    {
                        if ((Collide(this->position.x +  sign, this->position.y,node)))
                        {
                            if (moveCollideX(node)) break;
                            else this->position.x += sign;
                        }
                        else 
                        {
                            this->position.x += sign;
                        }
                        x -= sign;
                    }
                }
                else this->position.x += x;
            }
            if (y != 0)
            {
                if (sweep || Collide(this->position.x, this->position.y + y,node))
                {
                    sign = y > 0 ? 1 : -1;
                    while (y != 0)
                    {
                        if ((Collide(this->position.x, this->position.y + sign,node)))
                        {
                            if (moveCollideY(node)) break;
                            else this->position.y += sign;
                        }
                        else this->position.y += sign;
                        y -= sign;
                    }
                }
                else this->position.y += y;
            }

            for (auto child : node->children)
            {
                if (child->getType() == NODE2D)
                {
                    Node2D *n = dynamic_cast<Node2D *>(child);
                    if (n!=nullptr)
                    {
                        moveBy(x, y, n, sweep);
                    }
                }
    }
    }
    else
    {
        this->position.x += x;
        this->position.y += y;
    }
}

Vector2 Node2D::getWorldPosition(const Vector2 &v) 
{
    
    return transform.TransformCoords(v.x,v.y);

}
Vector2 Node2D::getWorldPosition(float x, float y) 
{
    
    return transform.TransformCoords(x,y);

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

float Node2D::getWorldRotation() const
{

    if (parent != nullptr)
    {
        if (parent->getType() == NODE2D)
        {
            Node2D *node_parent = dynamic_cast<Node2D *>(parent);
            if (node_parent!=nullptr)
            {
                return (node_parent->getWorldRotation() + rotation);
            } else
            {
                return rotation;
            }
        }
    }
    return rotation;
}

float Node2D::getWorldX() const
{

    if (parent != nullptr)
    {
        if (parent->getType() == NODE2D)
        {
            Node2D *node_parent = dynamic_cast<Node2D *>(parent);
            if (node_parent!=nullptr)
            {
                return (node_parent->getWorldX() + position.x);
            } else
            {
                return position.x;
            }
        }
    }
    return position.x;
}

float Node2D::getWorldY() const
{
    if (parent != nullptr)
    {
        if (parent->getType() == NODE2D)
        {
            Node2D *node_parent = dynamic_cast<Node2D *>(parent);
            if (node_parent!=nullptr)
            {
                return (node_parent->getWorldY() + position.y);
            } else
            {
                return position.y;
            }
        }
    }
    return position.y;
}

float Node2D::getWorldScaleX() const
{
    if (parent != nullptr)
    {
        if (parent->getType() == NODE2D)
        {
            Node2D *node_parent = dynamic_cast<Node2D *>(parent);
            if (node_parent!=nullptr)
            {
                return (node_parent->getWorldScaleX() * scale.x);
            } else
            {
                return scale.x;
            }
        }
    }
    return scale.x;
}

float Node2D::getWorldScaleY() const
{
    if (parent != nullptr)
    {
        if (parent->getType() == NODE2D)
        {
            Node2D *node_parent = dynamic_cast<Node2D *>(parent);
            if (node_parent!=nullptr)
            {
                return (node_parent->getWorldScaleY() * scale.y);
            } else
            {
                return scale.y;
            }
        }
    }
    return scale.y;
}

float Node2D::getWorldOriginX() const
{
    if (parent != nullptr)
    {
        if (parent->getType() == NODE2D)
        {
            Node2D *node_parent = dynamic_cast<Node2D *>(parent);
            if (node_parent!=nullptr)
            {
                return (node_parent->getWorldOriginX() + origin.x);
            } else
            {
                return origin.x;
            }
        }
    }
    return origin.x;
}

float Node2D::getWorldOriginY() const
{
    if (parent != nullptr)
    {
        if (parent->getType() == NODE2D)
        {
            Node2D *node_parent = dynamic_cast<Node2D *>(parent);
            if (node_parent!=nullptr)
            {
                return (node_parent->getWorldOriginY() + origin.y);
            } else
            {
                return origin.y;
            }
        }
    }
    return origin.y;
}


float Node2D::getX() const
{
    return position.x;
}

float Node2D::getY() const
{
    return position.y;
}

float Node2D::getRealX() const
{
    return getWorldOriginX() + getWorldX() - getWorldOriginX();
}
float Node2D::getRealY() const
{
    return getWorldOriginY() + getWorldY() - getWorldOriginY();
}



Matrix2D Node2D::getTransform() 
{
    Matrix2D local;
    local.Identity();
    if (skew.x == 0.0f && skew.y == 0.0f)
    {

        if (rotation == 0.0)
        {

            local.Set(scale.x, 0.0, 0.0, scale.y, position.x - origin.x * scale.x, position.y - origin.y * scale.y);
        }
        else
        {
            float spin = rotation * RAD;
            float acos = cos(spin );
            float asin = sin(spin );
            float a = scale.x * acos;
            float b = scale.x * asin;
            float c = scale.y * -asin;
            float d = scale.y * acos;
            float tx = position.x - origin.x * a - origin.y * c;
            float ty = position.y - origin.x * b - origin.y * d;

            local.Set(a, b, c, d, tx, ty);
        }
    }
    else
    {

        local.Identity();
        local.Scale(scale.x, scale.y);
        local.Skew(skew.x, skew.y);
        local.Rotate(rotation);
        local.Translate(position.x, position.y);

        if (origin.x != 0.0f || origin.y != 0.0f)
        {

            local.tx = position.x - local.a * origin.x - local.c * origin.y;
            local.ty = position.y - local.b * origin.x - local.d * origin.y;
        }
    }



    return local;


}

Matrix2D Node2D::getWorldTransform() 
{
    local_transform = getTransform();
    if (parent != nullptr)
    {
        if (parent->getType() == NODE2D || parent->getType() == SPRITE2D)
        {
            Node2D *node_parent = dynamic_cast<Node2D *>(parent);
            
        
            if (node_parent!=nullptr)
            {
                Matrix2D mat = node_parent->getWorldTransform();
                wordl_transform = Matrix2DMult(local_transform, mat);
                return wordl_transform;
            } else
            {
        
                return local_transform;
            }
        } else
        {
            return local_transform;
        }
    } 

    return local_transform;

   
}
