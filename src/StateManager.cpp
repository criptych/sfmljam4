////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "StateManager.hpp"
#include "State.hpp"

////////////////////////////////////////////////////////////////////////////////

StateManager::StateManager():
    m_topModal(0),
    m_topOpaque(0)
{
    sf::err() << "StateManager<" << this << ">::StateManager()\n";
}

StateManager::~StateManager()
{
    sf::err() << "StateManager<" << this << ">::~StateManager\n";
    while (popState());
}

void StateManager::pushState(State &state)
{
    return pushState(&state);
}

void StateManager::pushState(State *state)
{
    if (state->isModal())
    {
        m_topModal = m_states.size();

        if (!m_states.empty())
        {
            m_states.back()->onSuspend();
        }
    }

    if (state->isOpaque())
    {
        m_topOpaque = m_states.size();
    }

    if (state->m_mgr)
    {
        state->m_mgr->removeState(state);
    }

    sf::err() << "StateManager<" << this << ">::pushState(@" << state << ") [" << m_states.size() << "]\n";

    m_states.push_back(state);
    state->m_mgr = this;

    state->onEnter();
    state->onResume();
}

State *StateManager::popState()
{
    State *state = nullptr;

    if (!m_states.empty())
    {
        state = m_states.back();

        sf::err() << "StateManager<" << this << ">::popState()->@" << state << " [" << m_states.size() << "]\n";

        state->onSuspend();
        state->onExit();

        m_states.pop_back();
        state->m_mgr = nullptr;

        if (!m_states.empty())
        {
            m_states.back()->onResume();
        }

        updateTops();
    }

    return state;
}

void StateManager::popState(State &state)
{
    return popState(&state);
}

void StateManager::popState(State *state)
{
    if (getState() == state)
    {
        popState();
    }
}

void StateManager::removeState(State &state)
{
    return removeState(&state);
}

void StateManager::removeState(State *state)
{
    StateList::iterator end = m_states.end();
    StateList::iterator rem = std::remove(m_states.begin(), end, state);

    if (rem != end)
    {
        sf::err() << "StateManager<" << this << ">::removeState(@" << state << ") [" << m_states.size() << "]\n";

        if (state->m_mgr == this)
        {
            state->m_mgr = nullptr;
            state->onSuspend();
            state->onExit();
        }

        m_states.erase(rem, end);

        updateTops();
    }
}

State *StateManager::setState(State &state)
{
    return setState(&state);
}

State *StateManager::setState(State *state)
{
    State *old = popState();
    pushState(state);
    return old;
}

State *StateManager::getState() const
{
    State *state = nullptr;

    if (!m_states.empty())
    {
        state = m_states.back();
    }

    return state;
}

bool StateManager::handleEvent(const sf::Event &event)
{
    for (unsigned int i = m_states.size(); i > m_topModal; i--)
    {
        if (m_states[i-1]->handleEvent(event))
        {
            return true;
        }
    }

    return false;
}

void StateManager::update(sf::Time delta)
{
    for (unsigned int i = m_topModal, n = m_states.size(); i < n; i++)
    {
        m_states[i]->update(delta);
    }
}

void StateManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (unsigned int i = m_topOpaque, n = m_states.size(); i < n; i++)
    {
        m_states[i]->draw(target, states);
    }
}

void StateManager::updateTops()
{
    m_topModal = m_topOpaque = 0;

    unsigned int i = 0;

    for (State *state : m_states)
    {
        if ((i > m_topModal) && state->isModal())
        {
            m_topModal = i;
        }
        if ((i > m_topOpaque) && state->isOpaque())
        {
            m_topOpaque = i;
        }
        ++i;
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

