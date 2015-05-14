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
}

StateManager::~StateManager()
{
    sf::err() << "StateManager::~StateManager\n";
    while (popState())
    {
    }
}

void StateManager::pushState(State &state)
{
    State *top = m_states.empty() ? nullptr : m_states.back();

    if (state.isModal())
    {
        m_topModal = m_states.size();

        if (top)
        {
            top->onSuspend();
        }
    }
    if (state.isOpaque())
    {
        m_topOpaque = m_states.size();
    }

    m_states.push_back(&state);

    if (state.m_mgr)
    {
        state.m_mgr->popState(state);
    }

    state.m_mgr = this;

    state.onEnter();
}

State *StateManager::popState()
{
    State *last = m_states.empty() ? nullptr : m_states.back();

    if (last != nullptr)
    {
        sf::err() << "StateManager::popState()\n";

        last->onExit();
        m_states.pop_back();
        last->m_mgr = nullptr;

        State *top = m_states.empty() ? nullptr : m_states.back();

        if (top != nullptr)
        {
            top->onResume();
        }

        updateTops();
    }

    return last;
}

void StateManager::popState(State &state)
{
    for (unsigned int i = m_states.size(); i > 0; i--)
    {
        unsigned int j = i - 1;

        if (m_states[j] == &state)
        {
            sf::err() << "StateManager::popState(State&)\n";

            m_states.erase(m_states.begin() + j);
            state.m_mgr = nullptr;
            state.onSuspend();
            state.onExit();
            break;
        }
    }

    updateTops();
}

State *StateManager::setState(State &state)
{
    State *last = popState();
    pushState(state);
    return last;
}

State *StateManager::getState() const
{
    return m_states.empty() ? nullptr : m_states.back();
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
    for (unsigned int i = m_states.size(); i > m_topModal; i--)
    {
        m_states[i-1]->update(delta);
    }
}

void StateManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (unsigned int i = m_topOpaque; i < m_states.size(); i++)
    {
        target.draw(*m_states[i], states);
    }
}

void StateManager::updateTops()
{
    m_topModal = m_topOpaque = 0;

    for (unsigned int i = m_states.size(); i > 0; i--)
    {
        unsigned int j = i - 1;

        if ((j > m_topModal) && m_states[j]->isModal())
        {
            m_topModal = j;
        }
        if ((j > m_topOpaque) && m_states[j]->isOpaque())
        {
            m_topOpaque = j;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

