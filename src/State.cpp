////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "State.hpp"

#include "StateManager.hpp"

////////////////////////////////////////////////////////////////////////////////

State::State():
    m_mgr()
{
    sf::err() << "State<" << this << ">::State()\n";
}

State::~State()
{
    sf::err() << "State<" << this << ">::~State()\n";
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
        m_mgr->removeState(this);
    }
}

void State::pop()
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
    sf::err() << "State<" << this << ">::onEnter()\n";
}

void State::onSuspend()
{
    sf::err() << "State<" << this << ">::onSuspend()\n";
}

void State::onResume()
{
    sf::err() << "State<" << this << ">::onResume()\n";
}

void State::onExit()
{
    sf::err() << "State<" << this << ">::onExit()\n";
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

