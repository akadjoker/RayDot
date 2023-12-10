#pragma once
#include "Core.hpp"
#include "Assets.hpp"
#include "Node.hpp"
#include "Node2D.hpp"


struct MapTile
{
    bool flipX;
    bool flipY;
    int     id;
};

class API_EXPORT TileMap : public Node
{
public:
    TileMap(const std::string &graph,int width, int height, int tileWidth, int tileHeight, int spacing=0, int margin=0);
    TileMap(const std::string &graph,const std::string &name,int width, int height, int tileWidth, int tileHeight, int spacing=0, int margin=0);
    virtual ~TileMap();


    virtual bool InBounds(int x, int y) const;

    virtual int GetTile(int x, int y);
    virtual void SetTile(int x, int y, int tile);
    virtual void SetTile(int index, int tile);
    virtual Rectangle GetTileRect(float x, float y);


    virtual int GetWorldTile(float x, float y);
    virtual void SetWorldTile(float x, float y, int tile);

    void addTile(int tile);

    void Replace(int oldTile, int newTile);

    void LoadFromFile(const std::string &filename);
    void LoadFromArray(const int *tiles);
    void LoadFromString(const std::string &str,int shift);

    void SaveToFile(const std::string &filename);
    const std::string  &TilesToString() const;
    
    Node *CreateSolids(const std::string &name);
    Node *CreateSolids(const std::string &name, std::vector<int> values);

    void Clear();


    virtual void OnDraw(View *view) override;
    virtual void OnUpdate(double deltaTime) override;


    Rectangle getClip(int frame) const;

    void SetColor(const Color &color);



    void PaintRectangle(int x, int y, int w, int h, int id);
    void PaintCircle(int x, int y, int radius, int id);



private:
    std::vector<int> tiles;
    int tileWidth;
    int tileHeight;
    int spacing;
    int margin;
    int width;
    int height;
    int worldWidth;
    int worldHeight;
    Graph *graph;
    Color color;
};


