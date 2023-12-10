#include "NodeTileMap.hpp"
#include "RenderUtils.hpp"  
#include "Shape2D.hpp"
#include "Sprite2D.hpp"

#include <string>
#include <sstream>
#include <fstream>



TileMap::TileMap(const std::string &graph,int width, int height, int tileWidth, int tileHeight, int spacing, int margin): 
    Node("TileMap"), tileWidth(tileWidth), tileHeight(tileHeight), spacing(spacing), margin(margin), width(width), height(height)
{
    tiles.reserve(width * height);
    bound.Set(0, 0, width * tileWidth, height * tileHeight);
    type = TILELAYER;
    worldWidth = width * tileWidth;
    worldHeight = height * tileHeight;
    this->graph = Assets::Instance().getGraph(graph);
    color = WHITE;
    collidable = false;
}

TileMap::TileMap(const std::string &graph,const std::string &name, int width, int height, int tileWidth, int tileHeight, int spacing, int margin): 
    Node(name), tileWidth(tileWidth), tileHeight(tileHeight), spacing(spacing), margin(margin), width(width), height(height)
{
    tiles.reserve(width * height);
    bound.Set(0, 0, width * tileWidth, height * tileHeight);
    type = TILELAYER;
    worldWidth  = width * tileWidth;
    worldHeight = height * tileHeight;
    this->graph = Assets::Instance().getGraph(graph);
    color = WHITE;
    collidable = false;
}

TileMap::~TileMap()
{
}

bool TileMap::InBounds(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height;
}



int TileMap::GetTile(int x, int y)
{
    if (InBounds(x, y))
    {
        return tiles[y * width + x];
    }
    return -1;
}

void TileMap::SetTile(int x, int y, int tile)
{
    if (InBounds(x, y))
    {
        tiles[y * width + x] = tile;
    }
}

void TileMap::SetTile(int index, int tile)
{
    if (index >= 0 && index < width * height)
    {
        tiles[index] = tile;
    }
}

Rectangle TileMap::GetTileRect(float x, float y)
{
    Rectangle r;
    
    x = fmaxf(0, fminf(x, width * tileWidth));
    y = fmaxf(0, fminf(y, height * tileHeight));

    int px = x / tileWidth;
    int py = y / tileHeight;

    int tile = tiles[px + py * width] -1 ;
    if (tile == -1)
        return r;
    

    float tileX = px  * tileWidth;
    float tileY = py  * tileHeight;
    r.x = tileX;
    r.y = tileY;
    r.width  = tileWidth;
    r.height = tileHeight;
    return r;
 
}

int TileMap::GetWorldTile(float x, float y)
{
    int px =floor( x / tileWidth);
    int py =floor( y / tileHeight);  
    if (InBounds(px, py))
    {
        return tiles[px + py * width];
    }
    return -1;
    
}

void TileMap::SetWorldTile(float x, float y, int tile)
{
    int px =floor( x / tileWidth);
    int py =floor( y / tileHeight);  
    if (InBounds(px, py))
    {
        tiles[px + py * width] = tile;
    }
}

void TileMap::addTile(int tile)
{
    tiles.push_back(tile);
}

void TileMap::Replace(int oldTile, int newTile)
{
    for (int i = 0; i < (int)tiles.size(); i++)
    {
        if (tiles[i] == oldTile)
        {
            tiles[i] = newTile;
        }
    }
}

void TileMap::LoadFromString(const std::string &text, int shift)
{
    std::istringstream file(text);
    tiles.clear();
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);
        std::string cell;
        while (std::getline(lineStream, cell, ','))
        {
            int tile = std::stoi(cell) + shift;
            tiles.push_back(tile);
        }
    }
}

void TileMap::LoadFromFile(const std::string &filename)
{
    
    if (FileExists(filename.c_str()))
    {
    {
    char *buffer = LoadFileText(filename.c_str());

      

    if (buffer == nullptr)
    {
        Log(LOG_ERROR, " Reading  %s", filename.c_str());
        return;
    }
    
    std::string text(buffer);
    text.erase(std::remove_if(text.begin(), text.end(), [](char c) { return std::isspace(c); }), text.end());

    std::istringstream file(text);
    tiles.clear();


            std::string line;
            while (std::getline(file, line))
            {
                std::istringstream lineStream(line);
                std::string cell;
                while (std::getline(lineStream, cell, ','))
                {
                
                int tile = std::stoi(cell) - 1;
                tiles.push_back(tile);
                }
            }

        UnloadFileText(buffer);
    }
    }
}

const std::string &TileMap::TilesToString() const
{
    std::string str;
    for (int i = 0; i < (int)tiles.size(); i++)
    {
        str += std::to_string(tiles[i]);
        if (i < (int)tiles.size() - 1)
            str += ",";
    }
    return str;
    
}

void TileMap::LoadFromArray(const int *data)
{
    tiles.clear();
    int arraSize = (tileWidth * tileHeight);
    for (int i = 0; i < arraSize; i++)
    {
        tiles.push_back(data[i]);
    }
}



void TileMap::Clear()
{
    tiles.clear();
    tiles.reserve(width * height);
}

Rectangle TileMap::getClip(int frame) const
{
    Rectangle r{0, 0, 0, 0};
    
    if (!graph)
        return r;
    

    int tilesPerRow = floorf(graph->width / tileWidth);
    float tileX = floor((frame % tilesPerRow) * tileWidth);
    float tileY = floor((frame / tilesPerRow) * tileHeight);

    r.x = tileX;
    r.y = tileY;
    r.width  = tileWidth;
    r.height = tileHeight;
    return r;
 
}

void TileMap::PaintRectangle(int x, int y, int w, int h, int id)
{
    if ( !InBounds(x, y))
        return;
    for (int i = x; i < x + w; i++)
    {
        for (int j = y; j < y + h; j++)
        {
            SetTile(i, j, id);
        }
    }
}

void TileMap::PaintCircle(int x, int y, int radius, int id)
{

    if ( !InBounds(x, y))
        return;

    int rsq = radius * radius;
    for (int i = x - radius; i <= x + radius; ++i)
    {
        for (int j = y - radius; j <= y + radius; ++j)
        {
            int dx = i - x;
            int dy = j - y;
            if (dx * dx + dy * dy <= rsq)
            {
                SetTile(i, j, id);
            }
        }
    }
}

Node *TileMap::CreateSolids(const std::string &name)
{
    Node *node = new Node(name);
    node->bound.Set(0, 0, width, height);
    
    node->bound.Clear();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
               float  posX=(float)(j * tileWidth );
               float  posY=(float)(i * tileHeight);
               Rectangle tileRect = {posX, posY, tileWidth, tileHeight};
                    int tile= GetTile(j,i) ;
                    if (tile!=-1 && tile!=3)
                    {
                        // RectangleShape2D *solid = new RectangleShape2D(tileWidth, tileHeight, posX, posY);
                         //solid->bound.Set(posX, posY, tileWidth, tileHeight);

                        // node->addChild(solid);
                
                    }
            }
        }
    return node;
}

void TileMap::SetColor(const Color &color)
{
    this->color.r = color.r;
    this->color.g = color.g;
    this->color.b = color.b;
    this->color.a = color.a;
}

static bool isSolid(int tile, std::vector<int> &values)
{
    for (int i = 0; i < (int)values.size(); i++)
    {
        if (tile == values[i])
            return true;
    }
    return false;
}


Node *TileMap::CreateSolids(const std::string &name, std::vector<int> values)
{
    Node *node = new Node(name);
    node->bound.Set(0, 0, width * tileWidth, height * tileHeight);
    node->shape = new ListShape2D();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
               float  posX=(float)(j * tileWidth );
               float  posY=(float)(i * tileHeight);
               Rectangle tileRect = {posX, posY, tileWidth, tileHeight};
                    int tile= GetTile(j,i) ;
                    if (tile==-1)
                        continue;
                    if (isSolid(tile, values))
                    {
                       // Node *solid = new Node("Solid"); 
                            
                  //  (((ListShape2D*)node->shape))->add(new RectangleShape2D(tileWidth, tileHeight, posX, posY));

                       // solid->shape->setGroups( GROUP_TILE_SOLID, MASK_TILE_SOLID);
                       // node->addChild(solid);
                    }
            }
        }

          (((ListShape2D*)node->shape))->add(new RectangleShape2D(tileWidth, tileHeight*2, 360, 220));
    return node;
}

void TileMap::OnDraw(View *view)
{
    if (!visible)   return ;

    Node::OnDraw(view);
    if (!graph)
    {
        return;
    }
    
    if (!CheckCollisionRecs(view->getArea(), bound.getRectangle())) return;
                
    int count=0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
               float  posX=(float)(j * tileWidth );
               float  posY=(float)(i * tileHeight);
               Rectangle tileRect = {posX, posY, tileWidth, tileHeight};
            
                if (!CheckCollisionRecs(view->getArea(), tileRect)) continue;
                

                     int tile= GetTile(j,i) ;
                     if (tile!=-1)
                     {

                        RenderTile(graph->texture,
                                    posX,posY,
                                    tileWidth,tileHeight,
                                    getClip(tile),
                                    false,false,color,0);
             

                     // DrawRectangleLinesEx(tileRect, 1, RED);
                      count++;
                    }
                    //115 ;)

        }
    }
    //DrawText(TextFormat("TileLayer %d", count), 10, 10, 20, LIME);
}

void TileMap::OnUpdate(double deltaTime)
{
    Node::OnUpdate(deltaTime);
}

