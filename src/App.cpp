////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "App.hpp"

////////////////////////////////////////////////////////////////////////////////

App::App():
    m_defaultMode(960, 540, 32),
    m_desktopMode(sf::VideoMode::getDesktopMode()),
    m_windowMode(m_defaultMode),
    m_windowTitle(L"Untitled"),
    m_windowStyle(sf::Style::Default)
{
}

App::~App()
{
    m_window.close();
}

void App::run()
{
    sf::Clock clock;

    m_paused = false;

    onStart();

    m_startupTime = clock.restart();

    sf::err() << "App: startup took " << m_startupTime.asSeconds() << "s\n";

    m_lastFrameTime = m_targetFrameTime;
    m_fpsFrameTime = sf::Time::Zero;
    m_updateFps = m_renderFps = 0.f;
    m_updateFrameCount = m_renderFrameCount = 0;

    try
    {
        while (m_window.isOpen())
        {
            handleEvents();
            m_lastFrameTime = clock.restart();

            if (!m_paused)
            {
                // using fixed-step frames?
                if (m_targetFrameTime > sf::Time::Zero)
                {
                    m_runningFrameTime += m_lastFrameTime;

                    while (m_runningFrameTime >= m_targetFrameTime)
                    {
                        update(m_targetFrameTime);
                        m_updateFrameCount += 1;
                        m_runningFrameTime -= m_targetFrameTime;
                    }
                }
                else
                {
                    update(m_lastFrameTime);
                    m_updateFrameCount += 1;
                }

                m_renderFrameCount += 1;

                m_fpsFrameTime += m_lastFrameTime;

                static const sf::Time fpsInterval(sf::seconds(1.f));

                // if (m_fpsFrameTime >= m_fpsInterval)
                if (m_fpsFrameTime >= fpsInterval)
                {
                    float updateFps = float(m_updateFrameCount) / m_fpsFrameTime.asSeconds();
                    float renderFps = float(m_renderFrameCount) / m_fpsFrameTime.asSeconds();

                    m_updateFps = updateFps;// * 0.4f + m_updateFps * 0.6f;
                    m_renderFps = renderFps;// * 0.4f + m_renderFps * 0.6f;

                    char fpsString[64];
                    snprintf(fpsString, sizeof(fpsString), " [fps: R %.0f / U %.0f]", m_renderFps, m_updateFps);
                    m_window.setTitle(m_windowTitle + fpsString);

                    m_fpsFrameTime = sf::Time::Zero;
                    m_updateFrameCount = m_renderFrameCount = 0;
                }
            }

            render();
        }
    }
    catch (_Exit)
    {
        // safely ignored
    }

    onExit();
}

void App::exit()
{
    if (onExit())
    {
        m_window.close();
    }
}

void App::suspend()
{
    m_paused = true;
    onSuspend();
}

void App::resume()
{
    m_paused = false;
    onResume();
}

void App::setTargetFPS(float fps)
{
    if (fps > 0.f)
    {
        m_targetFrameTime = sf::seconds(1.f / fps);
    }
}

float App::getTargetFPS() const
{
    return (m_targetFrameTime > sf::Time::Zero) ?
        (1.f / m_targetFrameTime.asSeconds()) : 0.f;
}

float App::getUpdateFPS() const
{
    return m_updateFps;
}

float App::getRenderFPS() const
{
    return m_renderFps;
}

sf::RenderWindow &App::getWindow()
{
    return m_window;
}

void App::setWindowMode(const sf::VideoMode &mode)
{
    m_defaultMode = mode;

    if (m_window.isOpen())
    {
        createWindow();
    }
}

void App::setWindowTitle(const sf::String &title)
{
    m_windowTitle = title;

    if (m_window.isOpen())
    {
        m_window.setTitle(m_windowTitle);
    }
}

void App::setWindowStyle(sf::Uint32 style)
{
    m_windowStyle = style;
    m_windowMode = (style & sf::Style::Fullscreen) ? m_desktopMode : m_defaultMode;

    if (m_window.isOpen())
    {
        createWindow();
    }
}

void App::createWindow()
{
    m_window.create(m_windowMode, m_windowTitle, m_windowStyle);
}

void App::toggleFullscreen()
{
    setWindowStyle(m_windowStyle ^ sf::Style::Fullscreen);
}

void App::handleEvents()
{
    sf::Event event;

    while (m_window.pollEvent(event))
    {
        handleEvent(event);
    }
}

bool App::handleEvent(const sf::Event &event)
{
    if (onEvent(event) || StateManager::handleEvent(event))
    {
        return true;
    }

    switch (event.type)
    {
        case sf::Event::Closed:
        {
            exit();
            return true;
        }
        // case sf::Event::Resized:
        case sf::Event::LostFocus:
        {
            suspend();
            return true;
        }
        case sf::Event::GainedFocus:
        {
            resume();
            return true;
        }
        // case sf::Event::TextEntered:
        // case sf::Event::KeyPressed:
        // case sf::Event::KeyReleased:
        // case sf::Event::MouseWheelMoved:
        // case sf::Event::MouseWheelScrolled:
        // case sf::Event::MouseButtonPressed:
        // case sf::Event::MouseButtonReleased:
        // case sf::Event::MouseMoved:
        // case sf::Event::MouseEntered:
        // case sf::Event::MouseLeft:
        // case sf::Event::JoystickButtonPressed:
        // case sf::Event::JoystickButtonReleased:
        // case sf::Event::JoystickMoved:
        // case sf::Event::JoystickConnected:
        // case sf::Event::JoystickDisconnected:
        // case sf::Event::TouchBegan:
        // case sf::Event::TouchMoved:
        // case sf::Event::TouchEnded:
        // case sf::Event::SensorChanged:
        default:
        {
            return false;
        }
    }

    return false;
}

void App::update(sf::Time delta)
{
    onUpdate(delta);
    StateManager::update(delta);
}

void App::render()
{
    StateManager::draw(m_window, sf::RenderStates());
    m_window.display();
}

////////////////////////////////////////////////////////////////////////////////

void App::onStart()
{
    // sf::err() << "start\n";
    createWindow();
}

void App::onSuspend()
{
    // sf::err() << "suspended\n";
}

void App::onResume()
{
    // sf::err() << "resumed\n";
}

void App::onStop()
{
    // sf::err() << "stop\n";
}

bool App::onExit()
{
    // sf::err() << "exit\n";
    return true;
}

bool App::onEvent(const sf::Event &event)
{
    switch (event.type)
    {
        case sf::Event::KeyPressed:
        {
            switch (event.key.code)
            {
                case sf::Keyboard::F11:
                {
                    toggleFullscreen();
                    return true;
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

    return false;
}

void App::onUpdate(sf::Time delta)
{
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

