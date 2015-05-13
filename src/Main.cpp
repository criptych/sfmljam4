////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <physfs.h>

#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_rect_pack.h>
#include <stb_truetype.h>
#include <stb_perlin.h>

////////////////////////////////////////////////////////////////////////////////

#include "PhysicsFS.hpp"
#include "LuaState.hpp"

////////////////////////////////////////////////////////////////////////////////

extern "C"
int main (int argc, char **argv)
{
    PhysicsFS physfs(argv[0]);
    LuaState lua;

    sf::VideoMode desktopMode(sf::VideoMode::getDesktopMode());
    sf::VideoMode defaultMode(960, 540, 32);

    try
    {
        lua.loadSafeLibs();
        lua.doFile("init.lua");

        bool fullscreen = false;
        sf::VideoMode windowMode(defaultMode);
        sf::String windowTitle(L"Untitled");
        sf::Uint32 windowStyle(sf::Style::Titlebar | sf::Style::Close);

        sf::RenderWindow window(windowMode, windowTitle, windowStyle);

        sf::ConvexShape shape(4);
        shape.setPoint(0, sf::Vector2f(-1, -1));
        shape.setPoint(1, sf::Vector2f(-1,  1));
        shape.setPoint(2, sf::Vector2f( 1,  1));
        shape.setPoint(3, sf::Vector2f( 1, -1));
        shape.setOutlineThickness(0.05f);
        shape.setOutlineColor(sf::Color::Black);
        shape.setFillColor(sf::Color(160,160,160));
        shape.setScale(sf::Vector2f(100,100));
        shape.setPosition(sf::Vector2f(200,200));

        while (window.isOpen())
        {
            sf::Event event;

            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                    case sf::Event::Closed:
                    {
                        window.close();
                        break;
                    }

                    case sf::Event::KeyPressed:
                    {
                        switch (event.key.code)
                        {
                            case sf::Keyboard::F11:
                            {
                                fullscreen = !fullscreen;
                                if (fullscreen)
                                {
                                    window.create(desktopMode, windowTitle, windowStyle | sf::Style::Fullscreen);
                                }
                                else
                                {
                                    window.create(defaultMode, windowTitle, windowStyle);
                                }
                                break;
                            }

                            default:
                            {
                                break;
                            }
                        }
                        break;
                    }

                    default:
                    {
                        break;
                    }
                }
            }

            window.clear(sf::Color(0x6688aa));

            window.draw(shape);

            window.display();
        }
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

