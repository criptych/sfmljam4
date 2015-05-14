////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __APP_HPP__
#define __APP_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include "StateManager.hpp"

#include <SFML/Graphics.hpp>

////////////////////////////////////////////////////////////////////////////////

class App : public StateManager
{
public:
    static const class _Exit {} Exit;

public:
    App();
    virtual ~App();

    void run();
    void exit();

    void suspend();
    void resume();

    void setTargetFPS(float fps);
    float getTargetFPS() const;
    float getUpdateFPS() const;
    float getRenderFPS() const;

protected:
    virtual void onStart();
    virtual void onSuspend();
    virtual void onResume();
    virtual void onStop();

    virtual bool onExit();

    virtual bool onEvent(const sf::Event &event);
    virtual void onUpdate(sf::Time delta);

protected:
    sf::RenderWindow &getWindow();

    void setWindowMode(const sf::VideoMode &mode);
    void setWindowTitle(const sf::String &title);
    void setWindowStyle(sf::Uint32 style);
    void createWindow();
    void toggleFullscreen();

    // void pushState(State &state);
    // State *popState();
    // State *setState(State &state);

    void handleEvents();
    bool handleEvent(const sf::Event &event);
    void update(sf::Time delta);
    void render();

private:
    sf::VideoMode m_defaultMode;
    sf::VideoMode m_desktopMode;

    sf::VideoMode m_windowMode;
    sf::String m_windowTitle;
    sf::Uint32 m_windowStyle;

    sf::Time m_startupTime;
    sf::Time m_targetFrameTime;
    sf::Time m_lastFrameTime;
    sf::Time m_runningFrameTime;

    sf::Time m_fpsFrameTime;
    sf::Uint32 m_updateFrameCount;
    sf::Uint32 m_renderFrameCount;
    float m_updateFps;
    float m_renderFps;

    bool m_paused;
    bool m_exiting;

    sf::RenderWindow m_window;

    // std::vector<State*> m_states;
    // unsigned int m_topModal;
    // unsigned int m_topOpaque;
};

////////////////////////////////////////////////////////////////////////////////

#endif // __APP_HPP__

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

