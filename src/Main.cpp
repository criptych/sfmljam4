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
        static const unsigned int numSides[10] = {
            90, 3, 4, 5, 6, 7, 8, 10, 12, 20
        };

        m_shapes.resize(10);

        for (unsigned int i = 0; i < 10; i++)
        {
            int r = ((i+9)/5)%2, c = (i+9)%5;
            initShape(m_shapes[i], sf::Vector2f(80+150*c, 270+150*r), 50, numSides[i]);
        }
    }

protected:
    void initShape(sf::ConvexShape &shape, const sf::Vector2f &position, float radius, unsigned int sides = 0)
    {
        if (sides < 3) sides = 3;

        static const float PI = 3.14159265359f;

        float angle = PI / float(sides);
        float step = angle * 2.f;

        shape.setPointCount(sides);

        for (unsigned int i = 0; i < sides; i++)
        {
            shape.setPoint(i, sf::Vector2f(radius*std::cos(angle), radius*std::sin(angle)));
            angle += step;
        }

        shape.setOutlineThickness(5);
        shape.setOutlineColor(sf::Color::Black);
        shape.setFillColor(sf::Color(160,160,160));
        shape.setPosition(position);
    }

    void onUpdate(sf::Time delta)
    {
        for (sf::Shape &shape : m_shapes)
        {
            shape.rotate(90.f * delta.asSeconds());
        }
    }

    bool onEvent(const sf::Event &event)
    {
        switch (event.type)
        {
            case sf::Event::KeyPressed:
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Num0:
                    case sf::Keyboard::Num1:
                    case sf::Keyboard::Num2:
                    case sf::Keyboard::Num3:
                    case sf::Keyboard::Num4:
                    case sf::Keyboard::Num5:
                    case sf::Keyboard::Num6:
                    case sf::Keyboard::Num7:
                    case sf::Keyboard::Num8:
                    case sf::Keyboard::Num9:
                    {
                        m_shapes[event.key.code - sf::Keyboard::Num0].
                            setFillColor(sf::Color::Red);
                        return true;
                    }

                    default: break;
                }
                break;
            }

            case sf::Event::KeyReleased:
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Num0:
                    case sf::Keyboard::Num1:
                    case sf::Keyboard::Num2:
                    case sf::Keyboard::Num3:
                    case sf::Keyboard::Num4:
                    case sf::Keyboard::Num5:
                    case sf::Keyboard::Num6:
                    case sf::Keyboard::Num7:
                    case sf::Keyboard::Num8:
                    case sf::Keyboard::Num9:
                    {
                        m_shapes[event.key.code - sf::Keyboard::Num0].
                            setFillColor(sf::Color::Green);
                        return true;
                    }

                    default: break;
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

        for (const sf::Shape &shape : m_shapes)
        {
            target.draw(shape, states);
        }
    }

private:
    std::vector < sf::ConvexShape > m_shapes;
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
                    if (getState() != &m_pauseState)
                    {
                        pushState(m_pauseState);
                        return true;
                    }
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

