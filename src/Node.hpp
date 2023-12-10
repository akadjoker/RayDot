#pragma once
#include "Core.hpp"
#include "Utils.hpp"
#include "Math.hpp"


enum  NodeFlags
{
    NF_NONE=0,
    NF_SHOW_BOUND=1,
    NF_SHOW_ORIGIN=2,
    NF_SHOW_COLLISION=4,
};


enum NodeType
{
    NODE=0,
    NODE2D,
    SPRITE2D,
    TILELAYER,
    TILEHORTHO,
    TILEISO,
    TILEHEXA,
    SCENE2D,
};
class Shape2D;
class Scene;
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

    void setSize(float width, float height);

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

    int NumChildren() const;
    
    virtual bool Collide(Node *other);
    virtual bool Collide(float x, float y, Node *other);


    virtual bool moveCollideX(Node *other);
    virtual bool moveCollideY(Node *other);

   virtual  void OnReady();
   virtual  void OnUpdate(double deltaTime);
   virtual  void OnDestroy();
   virtual  void OnDraw(View *view);
   virtual  void OnEvent(const std::string &event, const std::string &data);
   virtual  void OnCollision(Node *other);


    std::string         name;
    Node                *parent{nullptr};
    bool                active;
    bool                visible;
    bool                done;
    int                 index{0};
    int                 tag{0};
    bool                collidable{true};
    Shape2D             *shape{nullptr};
    Scene               *scene{nullptr};
    AABB                 bound;
    NodeType             type;
    long                 flags{0};

    float width;
    float height;
protected:
    friend class Scene; 
    friend class Node2D;
    friend class Sprite2D;
    friend class Shape2D;
    friend class CircleShape2D;
    friend class RectangleShape2D;
    friend class PolyShape2D;
    friend class TileMap;

    virtual void update(double deltaTime);
    virtual void draw();
    virtual void ready();
    virtual void destroy();    
    virtual void event(const std::string &event, const std::string &data);
    virtual void collision(Node *other);
    virtual void updateBound();

    
    std::vector<Node *> children;


  
};