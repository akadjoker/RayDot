#pragma once
#include "Core.hpp"
#include "Utils.hpp"
#include "Scene.hpp"


class Game;

class API_EXPORT State
{
public:
    
    virtual ~State();
    State(const std::string &name);


    virtual void ready();
    virtual void update();
    virtual void render() ;
    virtual void destroy();

    void begin(int index=0);
    void end();

    
    View *CreateView(int x, int y, int width, int height);
    View *getDefaultView() const;
    const View *getView(int index);

    const std::string &getName() const;
    
     int  getWidth() ;
     int  getHeight() ;

private:


    friend class Game;

    void OnReady();
    void OnUpdate();
    void OnDestroy();
    void OnDraw();

    std::string name;
    int screenWidth;
    int screenHeight;
    
    View *defaultView;
    View *currentView;
    std::vector<View *> views;
    bool isReady{false};
  



};


class  API_EXPORT Game
{
    public :
        
        void Init(int width, int height, const std::string& title);
        void Quit();
        bool IsRunning() ;

        void Update();
        void Render();

        State *AddState(State *state);
        bool  ChangeState(const std::string &stateName);

        void setClearColor(Color color);
        Color getClearColor() const;

        bool containsState(const std::string &stateName) const;
        State *getState(const std::string &stateName) const;


        static Game &Instance();
    private:
        friend class State;
        State* m_currentState;
        State* m_nextState;
        std::unordered_map<std::string, State *> m_states;
        bool    m_running;
        Color clearColor;
        
    private:
        Game() ;
        virtual ~Game() ;

};
    