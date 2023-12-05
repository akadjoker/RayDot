#pragma once
#include "Core.hpp"

class Timer
{
public:
    Timer() ;
    void update();
    void start();
    void reset();
    void pause();
    void resume();
    double getDeltaTime() const { return deltaTime; }
    double getElapsedTime() const;
    bool   isPaused() const { return paused; }

private:
    double startTime;
    double lastFrameTime;
    double deltaTime;
    double pausedTime;
    bool paused;
};
