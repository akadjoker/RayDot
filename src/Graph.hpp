#pragma once
#include "Core.hpp"
#include "Utils.hpp"


class API_EXPORT Graph
{

public:

        Graph();
        Graph(const char* filePath);
        virtual ~Graph();

        Texture2D texture;      
        int width;
        int height;            

        
private:
    friend class Assets;
    std::string filename;
    std::string key;
    
    
};