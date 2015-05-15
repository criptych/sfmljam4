////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "State.hpp"

#include "StateManager.hpp"

////////////////////////////////////////////////////////////////////////////////

State::State():
    m_mgr()
{
}

State::~State()
{
    sf::err() << "State::~State\n";
    exit();
}

bool State::handleEvent(const sf::Event &event)
{
    return onEvent(event);
}

void State::update(sf::Time delta)
{
    onUpdate(delta);
}

void State::exit()
{
    if (m_mgr)
    {
        m_mgr->popState(this);
    }
}

void State::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
}

bool State::isOpaque() const
{
    return true;
}

bool State::isModal() const
{
    return true;
}

void State::onEnter()
{
}

void State::onSuspend()
{
}

void State::onResume()
{
}

void State::onExit()
{
}

bool State::onEvent(const sf::Event &event)
{
    return false;
}

void State::onUpdate(sf::Time delta)
{
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

