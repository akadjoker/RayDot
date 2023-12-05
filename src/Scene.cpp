#include "Scene.hpp"
#include "Shape2D.hpp"


Scene *Scene::instance = nullptr;

Scene *Scene::Instance()
{
    if (!Scene::instance)
        Scene::instance = new Scene();
    return Scene::instance;
}


Scene::Scene()
{
    if (!Scene::instance)
        Scene::instance = this;
    clearColor = RAYWHITE;
    timer.start();
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    defaultView = new View();
    defaultView->setViewPort(0, 0, (float)screenWidth, (float)screenHeight);   
    currentView = defaultView;
   // addView(defaultView);
    
    
}

Scene::~Scene()
{
   
}

void Scene::Update()
{
   
    timer.update();
    double dt = timer.getDeltaTime();

    for (int i=0;i<(int)children.size();i++)
    {
        Node *child = children[i];
        if (child->active)
            child->OnUpdate(dt);

    
        if (child->done)
        {
            to_remove.push_back(child);
        }
    }

    if (!to_add.empty())
    {
        for (Node *child : to_add)
        {
            addChild(child);
            child->OnReady();
        }
        to_add.clear();
    }
    if (!to_remove.empty())
    {
        for (Node *child : to_remove)
        {
            child->OnDestroy();
            removeChild(child);
            delete child;
            child = nullptr;
        }
        to_remove.clear();
    }

}

void Scene::removeChild(Node *child)
{
   
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        if (*it == child)
        {
            children.erase(it);
            break;
        }
    }

}

void Scene::addChild(Node *child)
{
    children.push_back(child);
}

void Scene::render(void)
{
   for (int i=0;i<(int)children.size();i++)
    {
        Node *child = children[i];
        if (child->visible)
        {
            objectRender++;
            child->OnDraw();
        }
    }
}

void Scene::Draw()
{
    ClearBackground(clearColor);
    objectRender=0;
    int count = (int)views.size();
    if (count>0)
    {   
        for (View *view : views)
        {
            
            view->screenHeight = screenHeight;
            view->begin();
            render();
            view->end();
        }
    }


    View *view = currentView;
    float x = 15;
    float y = 18;
    float s = 18;
    view->screenHeight = screenHeight;
    view->begin();
    if (count==0)
        render();   
 
    DrawRectangle(x-5, y-5, 170,80, BLACK);
    DrawFPS(x, y);
    DrawText(TextFormat("Objects: %i/%d", Count(),objectRender), x, y + 1 * s, s, LIME);
    DrawText(TextFormat("Elapsed time: %.2f", timer.getElapsedTime()), x, y + 2 * s, s, LIME);
    DrawText(TextFormat("Delta time: %f", timer.getDeltaTime()), x, y + 3 * s, s, LIME);
    view->end();
    
    
 
     
}

void Scene::Clear()
{
    delete defaultView;
    for (auto view : views)
    {
        delete view;
        view = nullptr;
    }

     for (auto child : children)
    {
        child->OnDestroy();
        delete child;
        child = nullptr;
    }
    children.clear();        

     if (!to_add.empty())
    {
        for (Node *child : to_add)
        {
            child->OnDestroy();
            delete child;
            child = nullptr;
        }
        to_add.clear();
    }
    if (!to_remove.empty())
    {
        for (Node *child : to_remove)
        {
            child->OnDestroy();
            delete child;
            child = nullptr;
        }
        to_remove.clear();
    }

}

void Scene::Add(Node *node)
{
    to_add.push_back(node);
}

void Scene::Remove(Node *node)
{
    to_remove.push_back(node);
}

Node *Scene::CirclePick(float x, float y, float radius)
{
    for (int i=0;i<(int)children.size();i++)
    {
        Node *child = children[i];
        if (child->active)
        {
           if (child->CirclePick(x,y,radius))
                return child;
        }
    }
    return nullptr;
}

Node *Scene::RectanglePick(float x, float y, float w, float h)
{
    for (int i=0;i<(int)children.size();i++)
    {
        Node *child = children[i];
        if (child->active)
        {
           if (child->RectanglePick(x,y,w,h))
                return child;
        }
    }
    return nullptr;
}

Node *Scene::MousePick()
{
    for (int i=0;i<(int)children.size();i++)
    {
        Node *child = children[i];
        if (child->active)
        {
           if (child->MousePick())
                return child;
        }
    }
    return nullptr;
}

Node *Scene::Pick(float x, float y)
{
    for (int i=0;i<(int)children.size();i++)
    {
        Node *child = children[i];
        if (child->active)
        {
           if (child->CirclePick(x,y,0.5f))
                return child;
        }
    }
    return nullptr;
    
}

void Scene::addView(View *view)
{
    if (!view)
        return; 

    views.push_back(view);
   
}


View *Scene::getDefaultView() const
{
    return defaultView;
}

int Scene::Count()
{
    return (int)children.size();
}

void Scene::setClearColor(Color color)
{
    clearColor = color;
}

Color Scene::getClearColor() const
{
    return clearColor;
}
