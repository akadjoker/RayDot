#pragma once
#include "Core.hpp"
#include "Utils.hpp"


enum NodeType
{
    NODE=0,
    NODE2D,
    SPRITE2D,
    SHAPE2D,
    CIRCLESHAPE2D,
    RECTANGLESHAPE2D,
    POLYSHAPE2D,
    SCENE2D,
};


class Node
{
public:
    Node();
    virtual ~Node();
    Node(const std::string &name);

    void removeChild(Node *child);
    void removeAllChildren(bool deleteChildren = true);
    void remove();

    void addChild(Node *child);
    
    void setParent(Node *parent);
    Node *getParent() const;

    bool moveUp();
    bool moveDown();
    
    std::vector<Node *> getChildren() const;

    Node *getChildByName(const std::string &name) const;
    Node *getChildByType(NodeType type) const;
    Node *getChildByIndex(int index) const;

    Node *CirclePick(float x, float y, float radius);
    Node *RectanglePick(float x, float y, float w, float h);
    Node *MousePick();

    bool contains(NodeType type) const;
    
    std::string getName() const;
    
    void setName(const std::string &name);
    NodeType getType() const;



   virtual  void OnReady();
   virtual  void OnUpdate(double deltaTime);
   virtual  void OnDestroy();
   virtual  void OnDraw();


    std::string         name;
    Node                *parent;
    bool                active;
    bool                visible;
    bool                done;

protected:
    friend class Scene; 
    virtual void update(double deltaTime);
    virtual void draw();
    virtual void ready();
    virtual void destroy();    
    

    NodeType            type;
    std::vector<Node *> children;
  
};