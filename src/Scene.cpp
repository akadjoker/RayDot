#include "Scene.hpp"
#include "Shape2D.hpp"
#include "Assets.hpp"
#include "Utils.hpp"
#include "NodeTileMap.hpp"
#include <raylib.h>
#include "tinyxml2.h"
#include <string>
#include <sstream>
#include <fstream>



Scene::Scene()
{
    
    timer.start();
}

Scene::~Scene()
{
    Clear();
    
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
            
            removeChild(child);
            child->OnDestroy();
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
            child->scene = nullptr;
            children.erase(it);
            break;
        }
    }

}

void Scene::addChild(Node *child)
{
    
    child->scene = this;
    children.push_back(child);
}



void Scene::Render(View *view)
{
    
    objectRender=0;
    for (int i=0;i<(int)children.size();i++)
    {
        Node *child = children[i];

        
    if (!CheckCollisionRecs(view->getArea(), child->bound.getRectangle())) continue;



        if (child->visible)
        {

            objectRender++;
            child->OnDraw(view);

            
        }
    }
    

 
    // DrawRectangle(x-5, y-5, 170,80, BLACK);
    // DrawFPS(x, y);
    // DrawText(TextFormat("Objects: %i/%d", Count(),objectRender), x, y + 1 * s, s, LIME);
    // DrawText(TextFormat("Elapsed time: %.2f", timer.getElapsedTime()), x, y + 2 * s, s, LIME);
    // DrawText(TextFormat("Delta time: %f", timer.getDeltaTime()), x, y + 3 * s, s, LIME);
    // view->end();
    //rlGetMatrixProjection(&projection);


 
     
}

void Scene::Clear()
{
   

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
    node->scene = this;
    to_add.push_back(node);
}

void Scene::Remove(Node *node)
{
    node->scene = nullptr;
    to_remove.push_back(node);
}

bool Scene::moveUp(Node *node)
{
     if (!node || children.empty()) return false;
        for (auto it = children.begin(); it != children.end(); ++it)
        {
            if (*it == node)
            {
                if (it != children.begin())
                {
                    std::iter_swap(it, it - 1);
                    return true;
                }
            }
        } 
        
        return false;
}

bool Scene::moveDown(Node *node)
{
        if (!node || children.empty()) return false;
        for (auto it = children.begin(); it != children.end(); ++it)
        {
            if (*it == node)
            {
                if (it != children.end() - 1)
                {
                    std::iter_swap(it, it + 1);
                    return true;
                }
            }
        }
    return false;
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

bool Scene::Collide(Node *node)
{
    for (int i=0;i<(int)children.size();i++)
    {
        Node *child = children[i];
        if (child->active && child->collidable)
        {
            if (node->Collide(child))
                return true;
        }
    }
    return false;
}

bool Scene::Collisions()
{
    
    int count = (int)children.size();
    for (int i=0; i <= count-1; i++)
    {
            for (int j = i + 1; j < count; j++)
            {
                Node *a = children[i];
                Node *b = children[j];
                if (a == b) continue;
                
                if (a->collidable && b->collidable )
                {
                    if (a->Collide(b))
                    {
                            a->OnCollision(b);
                            b->OnCollision(a);

                         //   return true;
                    }
                    if (b->Collide(a))
                    {
                         //   Log(LOG_INFO, "b vs a: %s with %s",a->name.c_str(),b->name.c_str());
                            b->OnCollision(a);
                            a->OnCollision(b);
                       //     return true;
                    }
                    

                }
            }
    }
    return true;
}




bool Scene::ImportTileMap(const std::string &fileName, const std::string &name)
{
  

  tinyxml2::XMLDocument document;
  document.LoadFile(fileName.c_str());

  


  tinyxml2::XMLElement* mapElem = document.FirstChildElement("map");
  if (!mapElem)        return false;

  int mTileWidth;
  int mTileHeight;
  int mWidth;
  int mHeight;
  int mfirstgid;
  

 std::string image;

  mTileWidth  = mapElem->IntAttribute("tilewidth");
  mTileHeight = mapElem->IntAttribute("tileheight");
  mWidth    =  mapElem->IntAttribute("width");
  mHeight   = mapElem->IntAttribute("height");
  mfirstgid = mapElem->IntAttribute("firstgid");

Node *solids = new Node(name);
solids->shape = new ListShape2D();
solids->bound.Set(0, 0, mWidth * mTileWidth, mHeight * mTileHeight);  


  Log(LOG_INFO,"Load tile map %s Tile (%d,%d) Map (%d,%d) %d",fileName.c_str(),mTileWidth,mTileHeight,mWidth,mHeight,mfirstgid);

  //solids->bound.Set(0, 0, mWidth * mTileWidth, mHeight * mTileHeight);


    tinyxml2::XMLElement* tileSetElem = mapElem->FirstChildElement("tileset");
    while(tileSetElem)
    {
         tinyxml2::XMLElement *imageElement = tileSetElem->FirstChildElement();

         if (imageElement->FindAttribute("source") !=nullptr)
         {
             image = imageElement->Attribute("source");
             Assets::Instance().loadGraph(image, image);
            tileSetElem = tileSetElem->NextSiblingElement("tileset");
         }
    }

    

    tinyxml2::XMLElement* layerElem = mapElem->FirstChildElement("layer");
    if (!layerElem)     return false;
    while (layerElem)
    {

        std::string layerName = layerElem->Attribute("name");
        int width     = layerElem->IntAttribute("width");
        int height    = layerElem->IntAttribute("height");
        float offsetx = layerElem->FloatAttribute("offsetx");
        float offsety = layerElem->FloatAttribute("offsety");

        TileMap *tileMap = new TileMap(image,layerName,width,height,mTileWidth,mTileHeight);
        
        
        Add(tileMap);

        int id = layerElem->IntAttribute("id");
        Log(LOG_INFO,"Layer %s %d ",layerName.c_str(),id);

        if (!layerElem->FirstChildElement("data")->FirstChildElement("tile"))
        {
            tinyxml2::XMLElement* data = layerElem->FirstChildElement("data");
            if (data->Attribute("encoding") != nullptr)
            {
                std::string encoding = data->Attribute("encoding");
                std::string dataStr = data->GetText();
                tileMap->LoadFromString(dataStr,-1);
            }
        }
        layerElem = layerElem->NextSiblingElement("layer");
    }

    Add(solids);

    tinyxml2::XMLElement* objElem = mapElem->FirstChildElement("objectgroup");
    if (objElem!=nullptr) 
    {
        
        while(objElem)
        {
            std::string layerName = objElem->Attribute("name");
            int id                = objElem->IntAttribute("id");
            Log(LOG_INFO,"Object %s %d ",layerName.c_str(),id);


            tinyxml2::XMLElement* obj = objElem->FirstChildElement("object");
            while(obj)
            {
                
                int id = obj->IntAttribute("id");

                float x = obj->FloatAttribute("x");
                float y = obj->FloatAttribute("y");
                float width = obj->FloatAttribute("width");
                float height = obj->FloatAttribute("height");


                ((ListShape2D*)solids->shape)->add(new RectangleShape2D(width,height,x,y)); 


                Log(LOG_INFO,"Object %d %f %f %f %f",id,x,y,width,height);

                obj = obj->NextSiblingElement("object");
            
            }

             objElem = objElem->NextSiblingElement("objectgroup");
            }
    }



    return true;
}

Node *Scene::Collide(Node *node, const std::string &name)
{
    for (int i=0;i<(int)children.size();i++)
    {
        Node *child = children[i];
        if (child->name != name) continue;
        if (child->active && child->collidable)
        {
            if (node->Collide(child))
                return child;
        }
       
    }
    return nullptr;
}

Node *Scene::Collide(float x, float y, Node *node, const std::string &name)
{
    for (int i=0;i<(int)children.size();i++)
    {
        Node *child = children[i];
        if (child->name != name) continue;
        if (child->active && child->collidable)
        {
            if (node->Collide(x,y,child))
                return child;
        }
               
    }
    return nullptr;    
}

Node *Scene::Find(const std::string &name)
{
    for (int i=0;i<(int)children.size();i++)
    {
        Node *child = children[i];
  //      Log(LOG_INFO, "Find: %s",child->name.c_str());
        if (child->name == name)
            return child;
    }
    for (int i=0;i<(int)to_add.size();i++)
    {
        Node *child = to_add[i];
//        Log(LOG_INFO, "Find: on queue %s",child->name.c_str());
        if (child->name == name)
            return child;
    }
    return nullptr;
    
}

int Scene::Count()
{
    return (int)children.size();
}

int Scene::RenderedObjects()
{
    return objectRender;
}
