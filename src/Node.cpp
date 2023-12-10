#include "Node.hpp"
#include "Node2D.hpp"
#include "Shape2D.hpp"
#include"Scene.hpp"

Node::Node()
{
    this->name = "Node";
    this->type = NODE;
    this->parent = nullptr;
    this->active = true;
    this->visible = true;
    this->done = false;  
    this->shape = nullptr;
    bound.Set(0, 0, 5, 5);
    
}


Node::Node(const std::string &name)
{
    this->name = name;
    this->type = NODE;
    this->parent = nullptr;
    this->active = true;
    this->visible = true;
    this->done = false;
    this->shape = nullptr;
    bound.Set(0, 0, 5, 5);
    
}

Node::~Node()
{
    if (shape!=nullptr)  delete shape;
    removeAllChildren(true);
}

void Node::addChild(Node *child)
{
    
    child->setParent(this);
    children.push_back(child);
    child->index = children.size() - 1;
    child->OnReady();
}

void Node::removeChild(Node *child)
{
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        if (*it == child)
        {
            child->OnDestroy();
            children.erase(it);
            child->parent = nullptr;
            break;
        }
    }
}

void Node::removeAllChildren(bool deleteChildren)
{
    for (auto child : children)
    {
        if (deleteChildren)
        {

            child->OnDestroy();
            child->parent = nullptr;
            delete child;
            child = nullptr;

        }
    }
    children.clear();        
            
}

void Node::remove()
{
    if (this->parent)
        this->parent->removeChild(this);
}

void Node::setParent(Node *parent)
{
    if (this->parent)
        this->parent->remove();
    this->parent = parent;
}

Node *Node::getParent() const
{
    return parent;
}

void Node::setSize(float width, float height)
{
    this->width  = width;
    this->height  = height;
}

bool Node::moveUp()
{
    if (this->parent)
    {
        for (auto it = this->parent->children.begin(); it != this->parent->children.end(); ++it)
        {
            if (*it == this)
            {
                if (it != this->parent->children.begin())
                {
                    std::iter_swap(it, it - 1);
                    return true;
                }
            }
        }
    }
    return false;
}

bool Node::moveDown()
{
    if (this->parent)
    {
        for (auto it = this->parent->children.begin(); it != this->parent->children.end(); ++it)
        {
            if (*it == this)
            {
                if (it != this->parent->children.end() - 1)
                {
                    std::iter_swap(it, it + 1);
                    return true;
                }
            }
        }
    }
   
    return false;
}

std::vector<Node *> Node::getChildren() const
{
    return children;
}

Node *Node::getChildByName(const std::string &name) const
{
    
    for (auto child : children)
    {
        if (child->name == name)
            return child;
    }
    return nullptr;

}

Node *Node::getChildByType(NodeType type) const
{
    for (auto child : children)
    {
        if (child->type == type)
            return child;
    }
    return nullptr;
}

Node *Node::getChildByIndex(int index) const
{
    if (index < 0 || index >= (int)children.size())
        return nullptr;
    return children[index];
}

Node *Node::CirclePick(float x, float y, float radius)
{
    // for (auto child : children)
    // {
    //     if (child->CirclePick(x, y, radius))
    //         return child;
    //     if (child->type == CIRCLESHAPE2D)
    //     {
    //         CircleShape2D *circle = dynamic_cast<CircleShape2D *>(child);
    //         if (circle->collide(x, y, radius))
    //             return child;
    //     } else 
    //     if (child->type == RECTANGLESHAPE2D)
    //     {
    //         RectangleShape2D *rectangle = dynamic_cast<RectangleShape2D *>(child);
    //         if (rectangle->collide(x, y, radius))
    //                 return child;
            
    //     }
    // }
   return nullptr;
}

Node *Node::RectanglePick(float x, float y, float w, float h)
{
    // for (auto child : children)
    // {
    //     if (child->RectanglePick(x, y, w, h))
    //         return child;
    //     if (child->type == CIRCLESHAPE2D)
    //     {
    //         CircleShape2D *circle = static_cast<CircleShape2D *>(child);
    //         if (circle->collide(x, y, w, h))
    //             return child;

    //     } else 
    //     if (child->type == RECTANGLESHAPE2D)
    //     {
    //         RectangleShape2D *rectangle = static_cast<RectangleShape2D *>(child);
    //         if (rectangle->collide(x, y, w, h))
    //             return child;
            
    //     }
    // }
    return nullptr;
}

Node *Node::MousePick()
{
    // Vector2 mouse = GetMousePosition();
    // DrawCircleLines(mouse.x, mouse.y, 5, RED);
    // for (auto child : children)
    // {
    //     if (child->MousePick())
    //          return child;
    //     if (child->type == CIRCLESHAPE2D)
    //     {
    //         CircleShape2D *circle = static_cast<CircleShape2D *>(child);
    //         if (circle->collide(mouse))
    //             return child;
         
    //     } else 
    //     if (child->type == RECTANGLESHAPE2D)
    //     {
    //         RectangleShape2D *rectangle = static_cast<RectangleShape2D *>(child);
    //         if (rectangle->collide(mouse))
    //             return child; 
         
    //     }
    // }
    return nullptr;
}

bool Node::contains(NodeType type) const
{
    for (auto child : children)
    {
        if (child->type == type)
            return true;
    }
    return false;
}

std::string Node::getName() const
{
    return name;
}

void Node::setName(const std::string &name)
{
    this->name = name;
}

NodeType Node::getType() const
{
    return type;
}

int Node::NumChildren() const
{
    return (int)children.size();
}

bool Node::Collide(Node *other)
{
     if (!shape || !other)  
    {
        // Log(LOG_WARNING, "Node::Collide: shape or other is null");
          return false;
    }

    if (other->shape!=nullptr) 
    {
        if (shape->can(other->shape))
        {
            if (shape->collide(other->shape))
            {
                OnCollision(other);
                other->OnCollision(this);
                return true;
            }
        }
    }
 

    for (auto child : other->children)
    {
         if (child->shape!=nullptr)
        {
            if (shape->can(child->shape))
            {
                if (shape->collide(child->shape))
                {
                    OnCollision(other);
                    other->OnCollision(this);
                    return true;
                }
            }
        }
    }



    return false;
}

bool Node::Collide(float x, float y, Node *other)
{
    if (!shape || !other)  
    {
       //   Log(LOG_WARNING, "Node::Collide: shape or other is null");
          return false;
    }
  
  
     if (other->shape!=nullptr) 
    {
        if (shape->collide(x,y,other->shape))
        {
            OnCollision(other);
            other->OnCollision(this);
            return true;
        }
    }

    for (auto child : other->children)
    {
        if (child->shape!=nullptr)
        {
            if (shape->collide(x,y,child->shape))
            {
                OnCollision(other);
                other->OnCollision(this);
                return true;
            }
        }
    }

    return false;
}

bool Node::moveCollideX(Node *other)
{
    (void)other;
    return true;
}

bool Node::moveCollideY(Node *other)
{
    (void)other;
    return true;
}

void Node::update(double deltaTime)
{
    (void)deltaTime;
}

void Node::draw()
{
}

void Node::ready()
{
}

void Node::destroy()
{
}

void Node::event(const std::string &event, const std::string &data)
{
    (void)event;
    (void)data;
}

void Node::collision(Node *other)
{
    (void)other;
}

void Node::updateBound()
{
}

void Node::OnReady()
{
    updateBound();
    ready();
    for (auto child : children)
        child->OnReady();

    
}

void Node::OnUpdate(double deltaTime)
{
    update(deltaTime);
    for (auto child : children)
        child->OnUpdate(deltaTime);
    
}

void Node::OnDestroy()
{
    for (auto child : children)
        child->OnDestroy();
    destroy();
}

void Node::OnDraw(View *view)
{
    draw();
    //bound.render();
    if (shape!=nullptr)
    {
      //  shape->render();
    }
    for (auto child : children)
        child->OnDraw(view);
}

void Node::OnEvent(const std::string &e, const std::string &data)
{
    event(e, data); 
    for (auto child : children)
        child->OnEvent(e, data);
}

void Node::OnCollision(Node *other)
{
    collision(other);
    for (auto child : children)
        child->OnCollision(other);
}
