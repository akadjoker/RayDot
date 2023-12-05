
#pragma once
#include "Core.hpp"
#include "Utils.hpp"
#include "Graph.hpp"



class API_EXPORT Assets
{
public:
    static Assets &Instance();

    Graph *getGraph(const std::string &key);
    
    bool hasGraph(const std::string &key);
    
    Graph *loadGraph(const std::string &key, const std::string &filepath);
    void unloadGraph(const std::string &key);
    
    void clear();
    
    Assets() {};
    Assets(const Assets &) = delete;
    Assets &operator=(const Assets &) = delete;

    std::unordered_map<std::string, Graph *> graphs;
};
