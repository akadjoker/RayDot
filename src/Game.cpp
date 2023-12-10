#include "Game.hpp"
#include "Assets.hpp"





State::~State()
{
     for (auto view : views)
    {
        delete view;
        view = nullptr;
    }
}

State::State(const std::string &name):name(name)
{
    screenWidth  = GetScreenWidth();
    screenHeight = GetScreenHeight();
    defaultView = CreateView(0, 0, screenWidth, screenHeight);
    currentView = defaultView;

}

void State::ready()
{
}

void State::update()
{
    
}

void State::render()
{

}

void State::destroy()
{
}

void State::begin(int index)
{
    if (index < 0 || index >= (int)views.size())
    {
        Log(LOG_WARNING, "Invalid view index %d", index);
        return;
    }
    currentView = views[index];
    currentView->begin();
}

void State::end()
{
    currentView->end();
    rlViewport(0,0,screenWidth,screenHeight);
}

Graph *State::addGraph(const std::string &name, const std::string &path)
{
  return Assets::Instance().loadGraph(name, path);
}

Graph *State::getGraph(const std::string &name)
{
    return Assets::Instance().getGraph(name);
}

View *State::CreateView(int x, int y, int width, int height)
{
    View *view = new View();
    view->setViewPort(x, y, width, height);
    views.push_back(view);
    return view;
}

View *State::getDefaultView() const
{
    return defaultView;
}

const View *State::getView(int index)
{
    if (index < 0 || index >= (int)views.size())
    {
        Log(LOG_WARNING, "Invalid view index %d", index);
        return nullptr;
    }
    return views[index];
}

const std::string &State::getName() const
{
    return name;
}

int State::getWidth()
{
    screenWidth  = GetScreenWidth();
    return screenWidth;
}

int State::getHeight()
{
    screenHeight = GetScreenHeight();
    return screenHeight;
}

void State::OnReady()
{
    if (isReady)
    {
        return;
    }
    ready();
    isReady = true;
}

void State::OnUpdate()
{
    if (!isReady)
    {
        OnReady();
        return;
    }
    update();
}

void State::OnDestroy()
{
    if (!isReady)
    {
        return;
    }
    isReady = false;
    destroy();
}

void State::OnDraw()
{
    if (!isReady)
    {
        return;
    }
    render();
}

Game &Game::Instance()
{
    static Game instance;
    return instance;
}

Game::Game()
{
    m_running = false;
    m_currentState = nullptr;
    m_nextState = nullptr;
    clearColor = RAYWHITE;
}

Game::~Game()
{
    if (m_running)
    {
        Quit();
    }
}

void Game::Init(int width, int height, const std::string &title)
{
    InitWindow(width, height, title.c_str());
    SetTargetFPS(60);

}

void Game::Quit()
{
    m_running = false;

    for (auto state : m_states)
    {
        delete state.second;
        state.second = nullptr;
    }
    m_states.clear();
   
    Assets::Instance().clear();
    CloseWindow(); 
}

bool Game::IsRunning() 
{
    m_running = !WindowShouldClose();
    return m_running;
}

void Game::Update()
{   
    if (m_nextState != nullptr)
    {
        if (m_currentState != nullptr)
        {
            m_currentState->OnDestroy();
        }
        m_currentState = m_nextState;
        m_currentState->OnReady();
        m_nextState = nullptr;
        
    }

    if (m_currentState != nullptr)
    {
        m_currentState->OnUpdate();
    }
}

void Game::Render()
{
  
    
    BeginDrawing();
    ClearBackground(clearColor);
    if (m_currentState != nullptr)
    {
        m_currentState->OnDraw();
    }
    DrawFPS(10, 10);
    EndDrawing();
}

State *Game::AddState(State *state)
{
    if (state == nullptr)
    {
        return nullptr;
    }
    if (m_states.find(state->getName()) != m_states.end())
    {
        Log(LOG_WARNING, "State %s already exists", state->getName().c_str());
        return nullptr;
    }
    m_states[state->getName()] = state;
    return state;
}

bool Game::ChangeState(const std::string &stateName)
{
    State *state = getState(stateName);
    if (state != nullptr)
    {
        if (m_currentState != nullptr)
        {
           if (m_currentState->getName() == stateName)
           {
               return false;
           }    
        }
        Log(LOG_INFO, "Changing state to %s", stateName.c_str());
        m_nextState = state;
        return true;
    }
    Log(LOG_WARNING, "State %s does not exist", stateName.c_str());
    return false;
}

Graph *Game::addGraph(const std::string &name, const std::string &path)
{
    return Assets::Instance().loadGraph(name, path);
}

void Game::setClearColor(Color color)
{
    clearColor = color;
}

Graph *Game::getGraph(const std::string &name)
{
    return Assets::Instance().getGraph(name);
}

Color Game::getClearColor() const
{
    return clearColor;
}

bool Game::containsState(const std::string &stateName) const
{
    auto it = m_states.find(stateName);
    if (it != m_states.end())
    {
        return true;
    }
    return false;
}

State *Game::getState(const std::string &stateName) const
{
    auto it = m_states.find(stateName);
    if (it != m_states.end())
    {
        return it->second;
    }
    return nullptr;
}
