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

void StateManager::pushState(const Handle<State> &state)
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

    m_states.push_back(state);

    if (state->m_mgr)
    {
        state->m_mgr->popState(state);
    }

    state->m_mgr = this;

    state->onEnter();
}

Handle<State> StateManager::popState()
{
    Handle<State> last;

    if (!m_states.empty())
    {
        last = m_states.back();

        sf::err() << "StateManager::popState()\n";

        last->onExit();
        m_states.pop_back();
        last->m_mgr = nullptr;

        if (!m_states.empty())
        {
            m_states.back()->onResume();
        }

        updateTops();
    }

    return last;
}

void StateManager::popState(const Handle<State> &state)
{
    for (unsigned int i = m_states.size(); i > 0; i--)
    {
        unsigned int j = i - 1;

        if (m_states[j] == state)
        {
            sf::err() << "StateManager::popState(State&)\n";

            m_states.erase(m_states.begin() + j);
            state->m_mgr = nullptr;
            state->onSuspend();
            state->onExit();
            break;
        }
    }

    updateTops();
}

Handle<State> StateManager::setState(const Handle<State> &state)
{
    const Handle<State> last = popState();
    pushState(state);
    return last;
}

Handle<State> StateManager::getState() const
{
    Handle<State> top;

    if (!m_states.empty())
    {
        top = m_states.back();
    }

    return top;
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
        target.draw(m_states[i], states);
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

