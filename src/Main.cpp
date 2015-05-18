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
    void onEnter()
    {
        sf::err() << "PauseState::onEnter()\n";
    }

    void onExit()
    {
        sf::err() << "PauseState::onEnter()\n";
    }

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
    void onEnter()
    {
        sf::err() << "MyState::onEnter()\n";
    }

    void onExit()
    {
        sf::err() << "MyState::onEnter()\n";
    }

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

        // m_myState = std::shared_ptr<State>(new MyState);
        // m_pauseState = std::shared_ptr<State>(new PauseState);
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
    // std::shared_ptr<State> m_pauseState;
    // std::shared_ptr<State> m_myState;
    PauseState m_pauseState;
    MyState m_myState;
};

////////////////////////////////////////////////////////////////////////////////

extern "C"
int main (int argc, char **argv)
{
    try
    {
        PhysicsFS physfs(argv[0]);

        try
        {
            physfs.mount("data.zip");
        }
        catch (PhysicsFS::Error)
        {
            physfs.mount("data/");
        }

        LuaState lua;

        if (!lua.tryDoFile("config.lua"))
        {
            sf::err() << "Config script error: " << lua.toString() << "\n";
        }

        lua.loadSafeLibs();

        lua.doFile("data/scripts/init.lua");

        MyApp app;
        app.run();
    }
    catch (PhysicsFS::Error &exc)
    {
        sf::err() << "PhysFS error: " << exc.what() << "\n";
    }
    catch (LuaState::Error &exc)
    {
        sf::err() << "Lua error: " << exc.what() << "\n";
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

