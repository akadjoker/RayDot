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


    virtual void OnDraw(View *view) override;

    
    Graph       *graph{nullptr};
    Rectangle   clip;
    Color       color;
    bool        flipX{false};
    bool        flipY{false};
};

class Frame
{
    public:
    Frame(Graph *graph, const std::string &name, float fps,int frameWidth, int frameHeight, std::vector<Vector2> positions);
    virtual ~Frame();
    Frame(const Frame &) = delete;
    Frame &operator=(const Frame &) = delete;
private:
    friend class AnimatedSprite2D;
    Graph       *graph{nullptr};
    std::string name;
    float       fps;
    int         frameWidth;
    int         frameHeight;
    int         count;
    std::vector<Vector2> positions;

};

class API_EXPORT AnimatedSprite2D : public Node2D
{

    public:
    AnimatedSprite2D();
    AnimatedSprite2D(const std::string &name);

    void addFrame(const std::string &name,const std::string &graph,float fps,int frameWidth, int frameHeight, int spacing=0 );

    void play(const std::string &name ,bool loop=false);
    void play(const std::string &name, int startFrame, int endFrame, bool loop=false);
    void stop();
    void reset();

    virtual void OnDraw(View *view) override;
    virtual void OnUpdate(double deltaTime) override;

    bool contains(const std::string &name);
    Frame *getFrame(const std::string &name);

    void setLoop(bool loop);

    bool IsPlaying(const std::string &name);
    bool IsStopped(const std::string &name);
    bool IsStopped();

 

    Rectangle   clip;
    Color       color;
    bool        flipX{false};
    bool        flipY{false};
    bool        loop{true};

    virtual ~AnimatedSprite2D();
    private:
        int     currentFrame{0};
        float   frameTime{0.0f};
        int     frameCounter{0};
        int     startFrame{0};
        int     endFrame{0};
        int     numberOfFrames{0};
        Frame   *current{nullptr};
        bool    playing{false};
        bool    stopped{false};
    
        std::unordered_map<std::string, Frame*> frames;

        bool isCurrentInRange(int start, int end);


};