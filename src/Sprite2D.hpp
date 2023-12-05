#pragma once
#include "Core.hpp"
#include "Assets.hpp"
#include "Node.hpp"
#include "Node2D.hpp"


class API_EXPORT Sprite2D : public Node2D
{
    public:
    Sprite2D();
    Sprite2D(const std::string &name);
    Sprite2D(const std::string &name, const std::string &graphName);
    
    virtual ~Sprite2D();


    virtual void OnDraw() override;


    Graph       *graph{nullptr};
    Rectangle   clip;
    Color       color;
    bool        flipX{false};
    bool        flipY{false};
};