#pragma once
#include "Core.hpp"
#include "Node.hpp"
#include "Node2D.hpp"
#include "Timer.hpp"




class API_EXPORT Scene 
{
public:
    virtual ~Scene();
    
    Scene();


    void Update();
    void Render(View* view);
    void Clear();

    void Add(Node *node);
    void Remove(Node *node);

    bool moveUp(Node *node);
    bool moveDown(Node *node);

    Node *CirclePick(float x, float y, float radius);
    Node *RectanglePick(float x, float y, float w, float h);
    Node *MousePick();
    Node *Pick(float x, float y);

    bool Collide(Node *node);
    bool Collisions();

    bool ImportTileMap(const std::string &filename, const std::string &name);

    
    Node*  Collide(Node *node, const std::string &name);  
    Node*  Collide(float x, float y,Node *node, const std::string &name);  

    Node *Find(const std::string &name);
    




    int Count();
    int RenderedObjects(); 

    void  setClearColor(Color color);
    Color getClearColor() const;
private:

    std::vector<Node *> to_add;
    std::vector<Node *> to_remove;

    Timer timer;
    int objectRender;
   


  
protected:
  friend class Node;
  std::vector<Node *> children;


  void removeChild(Node *child);
  void addChild(Node *child);


    

  void render(void);


};