#include "Node.hpp"
#include "Node2D.hpp"
#include "Shape2D.hpp"

Node::Node()
{
    this->name = "Node";
    this->type = NODE;
    this->parent = nullptr;
    this->active = true;
    this->visible = true;
    this->done = false;    
}

Node::~Node()
{
    removeAllChildren(true);
}

Node::Node(const std::string &name)
{
    this->name = name;
    this->type = NODE;
    this->parent = nullptr;
    this->active = true;
    this->visible = true;
    this->done = false;
}

void Node::addChild(Node *child)
{
    child->setParent(this);
    children.push_back(child);
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
    for (auto child : children)
    {
        if (child->CirclePick(x, y, radius))
            return child;
        if (child->type == CIRCLESHAPE2D)
        {
            CircleShape2D *circle = dynamic_cast<CircleShape2D *>(child);
            if (circle->collide(x, y, radius))
                return child;
        } else 
        if (child->type == RECTANGLESHAPE2D)
        {
            RectangleShape2D *rectangle = dynamic_cast<RectangleShape2D *>(child);
            if (rectangle->collide(x, y, radius))
                    return child;
            
        }
    }
   return nullptr;
}

Node *Node::RectanglePick(float x, float y, float w, float h)
{
    for (auto child : children)
    {
        if (child->RectanglePick(x, y, w, h))
            return child;
        if (child->type == CIRCLESHAPE2D)
        {
            CircleShape2D *circle = static_cast<CircleShape2D *>(child);
            if (circle->collide(x, y, w, h))
                return child;

        } else 
        if (child->type == RECTANGLESHAPE2D)
        {
            RectangleShape2D *rectangle = static_cast<RectangleShape2D *>(child);
            if (rectangle->collide(x, y, w, h))
                return child;
            
        }
    }
    return nullptr;
}

Node *Node::MousePick()
{
    Vector2 mouse = GetMousePosition();
    DrawCircleLines(mouse.x, mouse.y, 5, RED);
    for (auto child : children)
    {
        if (child->MousePick())
             return child;
        if (child->type == CIRCLESHAPE2D)
        {
            CircleShape2D *circle = static_cast<CircleShape2D *>(child);
            if (circle->collide(mouse))
                return child;
         
        } else 
        if (child->type == RECTANGLESHAPE2D)
        {
            RectangleShape2D *rectangle = static_cast<RectangleShape2D *>(child);
            if (rectangle->collide(mouse))
                return child; 
         
        }
    }
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

void Node::OnReady()
{
    ready();
    for (auto child : children)
        child->OnReady();
}

void Node::OnUpdate(double deltaTime)
{
    
    for (auto child : children)
        child->OnUpdate(deltaTime);
    update(deltaTime);
}

void Node::OnDestroy()
{
    
    for (auto child : children)
        child->OnDestroy();
    destroy();
}

void Node::OnDraw()
{
    draw();
    for (auto child : children)
        child->OnDraw();
}
