#include "Sprite2D.hpp"
#include "RenderUtils.hpp"

Sprite2D::Sprite2D(): Node2D("Sprite2D")
{
    this->graph = nullptr;
    this->clip = {0.0f, 0.0f, 0.0f, 0.0f};
    this->color = WHITE;
    this->flipX = false;
    this->flipY = false;
    
    type = SPRITE2D;
}

Sprite2D::Sprite2D(const std::string &name): Node2D(name)
{
    this->graph = nullptr;
    this->clip = {0.0f, 0.0f, 0.0f, 0.0f};
    this->color = WHITE;
    this->flipX = false;
    this->flipY = false;
    type = SPRITE2D;
}

Sprite2D::Sprite2D(const std::string &name, const std::string &graphName): Node2D(name)
{
    this->graph = Assets::Instance().getGraph(graphName);
    this->clip = {0.0f, 0.0f, 0.0f, 0.0f};
    if (this->graph == nullptr)
    {
        clip.width  = graph->width;
        clip.height = graph->height;
    }
    
    this->color = WHITE;
    this->flipX = false;
    this->flipY = false;
    type = SPRITE2D;
}

Sprite2D::~Sprite2D()
{
}


void Sprite2D::OnDraw(View *view)
{
    Node2D::OnDraw(view);
    if (graph != nullptr)
    {
        if (clip.width == 0.0f && clip.height == 0.0f)
        {
            clip.width = graph->width;
            clip.height = graph->height;
        }
      //  RenderMatrixFlipClip(graph->texture, clip.width, clip.height, clip, flipX, flipY, color, getWorldTransform(), BLEND_ALPHA);
        RenderTransformFlipClip(graph->texture, clip.width, clip.height, clip, flipX, flipY, color, transform, BLEND_ALPHA);
    }
}


Frame::Frame(Graph *graph,const std::string &name, float fps,int frameWidth, int frameHeight, std::vector<Vector2> positions)
{
    this->fps = fps;
    this->positions = positions;
    this->graph = graph;
    this->name = name;
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
    this->count = (int)positions.size();
}

Frame::~Frame()
{
}


AnimatedSprite2D::AnimatedSprite2D(): Node2D("AnimatedSprite2D")
{
color = WHITE;
loop = true;
}

AnimatedSprite2D::AnimatedSprite2D(const std::string &name): Node2D(name)
{
color = WHITE;
loop = true;
}





void AnimatedSprite2D::addFrame(const std::string &name,const std::string &g,float fps,int frameWidth, int frameHeight, int spacing )
{
    std::vector<Vector2> frames;
    Graph *graph = Assets::Instance().getGraph(g);
    if (!graph)
    {
        Log(LOG_ERROR,"AnimatedSprite2D::addFrame: graph not found");
        return;
    }
    int columns  = graph->width / frameWidth;
    int rows     = graph->height / frameHeight;

    int numberOfFrames = columns * rows;
    for (int i = 0; i < numberOfFrames; i++)
    {
        int x = (i % columns) * frameWidth;
        int y = floor(i / columns) * frameHeight;

        if (spacing > 0)
        {
            x += spacing + (spacing * i % columns);
            y += spacing + (spacing * floor(i / columns));
        }
        frames.push_back({(float)x, (float)y});
        
    }
    Log(LOG_INFO, "AnimatedSprite2D::addFrame: %s %d %d %d %d %d", name.c_str(), columns,rows,frameWidth,frameHeight,numberOfFrames);
    Frame *frame = new Frame(graph,name, 1000.0f / fps,frameWidth,frameHeight,frames);
    this->frames[name] = frame;    
}

void AnimatedSprite2D::play(const std::string &name,bool loop)
{

    std::string lastName;
    if (current!=nullptr)
    {
      lastName=current->name;
    }
    current = getFrame(name);
    if (current == nullptr)
    {
        Log(LOG_ERROR,"AnimatedSprite2D::play: frame not found");
        return;
    }

    if (name == lastName && !loop)
    {
         return;
    }
    
   if (name != lastName || !playing)
    {
        this->loop = loop;
        playing = true;
        stopped = false;
        frameTime = 0.0f;
        frameCounter = 0;
        startFrame = 0;
        currentFrame = startFrame;
        endFrame     = current->count -1;
        numberOfFrames = endFrame - startFrame -1;

    //    Log(LOG_INFO, "AnimatedSprite2D::play: %s %d %d %d", name.c_str(), currentFrame, numberOfFrames ,endFrame);

        clip.x = current->positions[currentFrame].x;
        clip.y = current->positions[currentFrame].y;
        clip.width  = current->frameWidth;   
        clip.height = current->frameHeight;
    }
}

void AnimatedSprite2D::play(const std::string &name, int startFrame, int endFrame, bool loop)
{
   std::string lastName;
    if (current!=nullptr)
    {
      lastName=current->name;
    }
    current = getFrame(name);
    if (current == nullptr)
    {
        Log(LOG_ERROR,"AnimatedSprite2D::play: frame not found");
        return;
    }

    if (name == lastName && !loop)
    {
         return;
    }
    
  

    if ((!isCurrentInRange(startFrame, endFrame)) && ((startFrame >= 0 && endFrame < (int)current->positions.size()) || (stopped)))
//    if ( (!isCurrentInRange(startFrame,endFrame)) &&  (startFrame >= 0 && endFrame <(int) current->positions.size()) || (stopped) ) 
    {
        
        playing = true;
        stopped = false;
        frameCounter = 0;
        frameTime = 0.0f;
        this->startFrame = startFrame;
        this->endFrame = endFrame;
        this->loop = loop;
        numberOfFrames = endFrame - startFrame -1;
        currentFrame = startFrame;
        clip.x = current->positions[currentFrame].x;
        clip.y = current->positions[currentFrame].y;
        clip.width  = current->frameWidth;   
        clip.height = current->frameHeight;
    } 
}

void AnimatedSprite2D::stop()
{
    if (current == nullptr || playing == false)
    {
        return;
    }
    playing = false;
    stopped = true;
}

void AnimatedSprite2D::reset()
{
     if (current == nullptr)
    {
        return;
    }
    current = nullptr;
    playing = false;
    frameCounter = 0;
    startFrame = 0;
    endFrame = 0;
    numberOfFrames = 0;
    stopped = false;
    currentFrame = 0;
    frameTime = 0.0f;
}

void AnimatedSprite2D::OnDraw(View *view)
{
    Node2D::OnDraw(view);
    if (current == nullptr) return;
    if (!current->graph) return ;

    
    

    clip.x = current->positions[currentFrame].x;
    clip.y = current->positions[currentFrame].y;
    clip.width  = current->frameWidth;   
    clip.height = current->frameHeight;

    if (stopped && !this->loop)
    {
        clip.x = current->positions[endFrame].x;
        clip.y = current->positions[endFrame].y;
        clip.width  = current->frameWidth;   
        clip.height = current->frameHeight;
    }
    

    RenderTransformFlipClip(current->graph->texture, clip.width, clip.height, clip, flipX, flipY, color, transform, BLEND_ALPHA);

    DrawText(TextFormat("%d  %d  ",currentFrame,numberOfFrames),300,300,20,RED);

}

void AnimatedSprite2D::OnUpdate(double deltaTime)
{
    Node2D::OnUpdate(deltaTime);
    if (current == nullptr)    return;
    if (!current->graph)                            return ;
    if (stopped)                                  return;
    
    frameTime += (deltaTime * 1000);
    if (frameTime >= current->fps)
    {
        frameTime = 0.0f;
        currentFrame = startFrame + frameCounter;
        frameCounter++; 
        if (frameCounter > numberOfFrames)
        {
            if (loop) frameCounter = 0;
        }
        

        if (currentFrame > endFrame)
        {
            if (loop==true)
            {
                currentFrame = startFrame;
                stopped = false;
                playing = true;    
              //  Log(LOG_INFO,"AnimatedSprite2D::OnUpdate: restart %d  %s ", currentFrame , current->name.c_str());
            }
            else
            {
                currentFrame = endFrame;
                playing = false;
                stopped = true;
              //  Log(LOG_INFO,"AnimatedSprite2D::OnUpdate: stop %d  %s ", currentFrame , current->name.c_str());
            }
        }
    }
    //Log(LOG_INFO,"AnimatedSprite2D::OnUpdate: %d %d %f",currentFrame,numberOfFrames,frameTime);
}


AnimatedSprite2D::~AnimatedSprite2D()
{

     for (auto it = frames.begin(); it != frames.end(); it++)
    {
        delete it->second;
    }
    frames.clear();

}

bool AnimatedSprite2D::isCurrentInRange(int start, int end)
{
    if (currentFrame >= start && currentFrame <= end)
    {
        return true;
    }
    return false; 
}

bool AnimatedSprite2D::contains(const std::string &name)
{
    auto it = frames.find(name);
    if (it != frames.end())
    {
        return true;
    }
    return false;
}

Frame *AnimatedSprite2D::getFrame(const std::string &name)
{
    auto it = frames.find(name);
    if (it != frames.end())
    {
        return it->second;
    }
    return nullptr;
}

void AnimatedSprite2D::setLoop(bool loop)
{
    this->loop = loop;
}

bool AnimatedSprite2D::IsPlaying(const std::string &name)
{
    if (current == nullptr)
    {
        return false;
    }
    if (current->name == name && playing==true && stopped==false)
    {
        return true;
    }
    return false;
}

bool AnimatedSprite2D::IsStopped(const std::string &name)
{
    if (current == nullptr)
    {
        return false;
    }
    if (current->name == name && stopped==true)
    {
        return true;
    }
    return false;
}

bool AnimatedSprite2D::IsStopped()
{
    if (current == nullptr)
    {
        return false;
    }
    if (stopped==true)
    {
        return true;
    }
    return false;
}
