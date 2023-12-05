#include "Assets.hpp"
#include "Graph.hpp"

Graph::Graph()
{
}

Graph::Graph(const char *filePath)
{
        texture = LoadTexture(filePath);
        width = texture.width;
        height = texture.height;
        filename = filePath;
}

Graph::~Graph()
{
}
