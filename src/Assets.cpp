#include "Assets.hpp"



Assets &Assets::Instance()
{
    static Assets instance;
    return instance;
}

Graph *Assets::getGraph(const std::string &key)
{
    auto it = graphs.find(key);
    if (it != graphs.end())
    {
        return it->second;
    }
    
    return nullptr;
}

bool Assets::hasGraph(const std::string &key)
{
    auto it = graphs.find(key);
    if (it != graphs.end())
    {
        return true;
    }
    Log(LOG_WARNING, "Graph %s not found", key.c_str());
    return false;
}

Graph *Assets::loadGraph(const std::string &key, const std::string &filepath)
{
    Graph *graph = getGraph(key);
    if (graph != nullptr)
    {
        return graph;
    }

    if (FileExists(filepath.c_str()))
    {
        graph = new Graph(filepath.c_str());
    }
    else if (FileExists((std::string("assets/") + filepath).c_str()))
    {
        graph = new Graph((std::string("assets/") + filepath).c_str());
    }
    else if (FileExists((std::string("assets/images/") + filepath).c_str()))
    {
        graph = new Graph((std::string("assets/images/") + filepath).c_str());
    }
    else if (FileExists((std::string("assets/textures/") + filepath).c_str()))
    {
        graph = new Graph((std::string("assets/textures/") + filepath).c_str());
    }
    else if (FileExists((std::string("assets/levels/") + filepath).c_str()))
    {
        graph = new Graph((std::string("assets/levels/") + filepath).c_str());
    }
    else if (FileExists((std::string("../assets/levels/") + filepath).c_str()))
    {
        graph = new Graph((std::string("../assets/levels/") + filepath).c_str());
    }
    else if (FileExists((std::string("../assets/images/") + filepath).c_str()))
    {
        graph = new Graph((std::string("..assets/images/") + filepath).c_str());
    }
    else if (FileExists((std::string("../assets/textures/") + filepath).c_str()))
    {
        graph = new Graph((std::string("../assets/textures/") + filepath).c_str());
    }
    else
    {
        Log(LOG_ERROR, "Failed to load  image %s", filepath.c_str());
        return nullptr;
    }

    // if (FileExists(filepath.c_str()) == false)
    // {
    //     Log(LOG_WARNING, "File %s not found", filepath.c_str());
    //     return nullptr;
    // }

    graph->key = key;
    graphs[key] = graph;
    return graph;
}

void Assets::unloadGraph(const std::string &key)
{
    auto it = graphs.find(key);
    if (it != graphs.end())
    {
       
        UnloadTexture(it->second->texture);

        graphs.erase(it);
    }
}
void Assets::clear()
{
    for (auto &graph : graphs)
    {
        // TraceLog(LOG_INFO, "Unloading graph %s", graph.second->key.c_str());
        UnloadTexture(graph.second->texture);
        delete graph.second;
    }
    graphs.clear();
}