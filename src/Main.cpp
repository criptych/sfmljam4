////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

////////////////////////////////////////////////////////////////////////////////

#include "PhysicsFS.hpp"
#include "LuaState.hpp"

#include "App.hpp"
#include "State.hpp"

#include <cmath>

////////////////////////////////////////////////////////////////////////////////

class PauseState : public State
{
public:
    PauseState() {}

protected:
    void onUpdate(sf::Time delta)
    {
    }

    bool onEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Return)
            {
                throw App::Exit;
            }
            else if (event.key.code == sf::Keyboard::Escape)
            {
                exit();
            }
        }

        return false;
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        sf::RectangleShape rect(sf::Vector2f(target.getSize()));
        rect.setFillColor(sf::Color(0,0,0,128));
        target.draw(rect, states);
    }

    bool isOpaque() const { return false; }
};

////////////////////////////////////////////////////////////////////////////////

class MyState : public State
{
public:
    MyState()
    {
        initShape(360);
    }

protected:
    void initShape(unsigned int n = 5)
    {
        if (n < 3) n = 3;

        static const float PI = 3.14159265359f;

        float angle = PI / float(n);
        float step = angle * 2.f;

        m_shape.setPointCount(n);

        for (unsigned int i = 0; i < n; i++)
        {
            m_shape.setPoint(i, sf::Vector2f(100.f*std::cos(angle), 100.f*std::sin(angle)));
            angle += step;
        }

        // m_shape.setPoint(0, sf::Vector2f(-100, -100));
        // m_shape.setPoint(1, sf::Vector2f(-100,  100));
        // m_shape.setPoint(2, sf::Vector2f( 100,  100));
        // m_shape.setPoint(3, sf::Vector2f( 100, -100));

        m_shape.setOutlineThickness(5);
        m_shape.setOutlineColor(sf::Color::Black);
        m_shape.setFillColor(sf::Color(160,160,160));
        m_shape.setPosition(sf::Vector2f(200,200));
    }

    void onUpdate(sf::Time delta)
    {
        m_shape.rotate(90.f * delta.asSeconds());
    }

    bool onEvent(const sf::Event &event)
    {
        switch (event.type)
        {
            case sf::Event::KeyPressed:
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    m_shape.setFillColor(sf::Color::Red);
                    return true;
                }
                break;
            }
            case sf::Event::KeyReleased:
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    m_shape.setFillColor(sf::Color::Green);
                    return true;
                }
                break;
            }
            default: break;
        }

        return false;
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.clear(sf::Color(0x6688aa));
        target.draw(m_shape, states);
    }

private:
    sf::ConvexShape m_shape;
};

class MyApp : public App
{
public:
    MyApp()
    {
        setTargetFPS(120.f);
        setWindowStyle(sf::Style::Titlebar | sf::Style::Close);
    }

protected:
    void onStart()
    {
        createWindow();
        pushState(m_myState);
    }

    bool onEvent(const sf::Event &event)
    {
        switch (event.type)
        {
            case sf::Event::KeyPressed:
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    pushState(m_pauseState);
                    return true;
                }
                break;
            }
            default: break;
        }

        return false;
    }

private:
    PauseState m_pauseState;
    MyState m_myState;
};

////////////////////////////////////////////////////////////////////////////////

extern "C"
int main (int argc, char **argv)
{
    PhysicsFS physfs(argv[0]);

    try
    {
        LuaState lua;

        try
        {
            lua.doFile("config.lua");
        }
        catch (LuaState::Error &exc)
        {
            sf::err() << "Config script error: " << exc.what() << "\n";
        }

        lua.loadSafeLibs();

        try
        {
            lua.doFile("init.lua");
        }
        catch (LuaState::Error &exc)
        {
            sf::err() << "Init script error: " << exc.what() << "\n";
            throw;
        }

        MyApp app;
        app.run();
    }
    catch (std::exception &exc)
    {
        sf::err() << "Unhandled exception: " << exc.what() << "\n";
    }

    return(0);
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

