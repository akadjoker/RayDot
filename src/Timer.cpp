#include "Timer.hpp"    
#include "Utils.hpp"


    Timer::Timer() : startTime(GetTime()), lastFrameTime(0), deltaTime(0), pausedTime(0), paused(false)
    {
    }

    void Timer::update()
    {
        if (!paused)
        {
            double currentTime = GetTime();
            deltaTime =GetFrameTime();// currentTime - lastFrameTime;
            lastFrameTime = currentTime;
        }
        else
        {
            deltaTime = 0;
        }
    }

    void Timer::start()
    {
        startTime = GetTime();
        lastFrameTime = 0;
        deltaTime = 0;
        pausedTime = 0;
        paused = false;
    }

    void Timer::reset()
    {
        startTime = GetTime();
        pausedTime = 0;
        paused = false;
    }
    void Timer::pause()
    {
        if (!paused)
        {
            pausedTime = GetTime() - lastFrameTime;
            paused = true;
        }
    }

    void Timer::resume()
    {
        if (paused)
        {
            lastFrameTime = GetTime() - pausedTime;
            paused = false;
        }
    }

    

    double Timer::getElapsedTime() const
    {
        if (paused)
        {
            return pausedTime - startTime;
        }
        else
        {
            return GetTime() - startTime;
        }
    }

