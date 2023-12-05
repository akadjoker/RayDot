#pragma once
#include "Core.hpp"
#include "Node.hpp"
#include "Node2D.hpp"
#include "Timer.hpp"




class API_EXPORT Scene 
{
public:
    Scene();
    virtual ~Scene();
    static Scene *instance;
    static Scene *Instance();


    Scene &operator=(const Scene &) = delete;

    void Update();
    void Draw();
    void Clear();

    void Add(Node *node);
    void Remove(Node *node);

    Node *CirclePick(float x, float y, float radius);
    Node *RectanglePick(float x, float y, float w, float h);
    Node *MousePick();
    Node *Pick(float x, float y);
    

    void addView(View *view);

    View *getDefaultView() const;


    int Count();

    void  setClearColor(Color color);
    Color getClearColor() const;
private:
    Color clearColor;    
    std::vector<Node *> to_add;
    std::vector<Node *> to_remove;

    Timer timer;
    int objectRender;

protected:
  
  int screenWidth;
  int screenHeight;
  View *defaultView;
  View *currentView;
  View *newView{nullptr};
  std::vector<View *> views;
  std::vector<Node *> children;


  void removeChild(Node *child);
  void addChild(Node *child);


    

  void render(void);


};