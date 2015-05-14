////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "StateManager.hpp"

////////////////////////////////////////////////////////////////////////////////

StateManager::StateManager():
    m_topModal(0),
    m_topOpaque(0)
{
}

StateManager::~StateManager()
{
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

    if (last)
    {
        last->onExit();
        m_states.pop_back();
        last->m_mgr = nullptr;
    }

    State *top = m_states.empty() ? nullptr : m_states.back();

    if (last && top && last->isModal())
    {
        top->onResume();
    }

    updateTops();

    return last;
}

void StateManager::popState(State &state)
{
    // StateList::iterator i, j = m_states.end();

    // for (i = m_states.begin(); i != j; i++)
    for (unsigned int i = m_states.size(); i > 0; i--)
    {
        // if (*i == &state)
        if (m_states[i] == &state)
        {
            m_states.erase(m_states.begin() + i);
            // m_states.erase(i);
            // i--, j--;
            state.m_mgr = nullptr;
            state.onSuspend();
            state.onExit();
        }
    }
}

State *StateManager::setState(State &state)
{
    State *last = popState();
    pushState(state);
    return last;
}

bool StateManager::handleEvent(const sf::Event &event)
{
    // StateList::reverse_iterator i, j = m_states.rend() - m_topModal;

    for (unsigned int i = m_states.size(); i > m_topModal; i--)
    // for (i = m_states.rbegin(); i != j; i++)
    {
        try
        {
            // State *state = *i;

            // if (state->handleEvent(event))
            // {
                // return true;
            // }
            if (m_states[i-1]->handleEvent(event))
            {
                return true;
            }
        }
        catch (State::_Exit)
        {
            // m_states.erase(i);
            m_states.erase(m_states.begin() + i);
        }
    }

    return false;
}

void StateManager::update(sf::Time delta)
{
    // StateList::iterator i, j = m_states.end();

    for (unsigned int i = m_topModal; i < m_states.size(); i++)
    // for (i = m_states.begin() + m_topModal; i != j; i++)
    {
        try
        {
            // State *state = *i;
            // state->update(delta);
            m_states[i]->update(delta);
        }
        catch (State::_Exit)
        {
            // m_states.erase(i);
            // i--; j--;
            m_states.erase(m_states.begin() + i);
            i--;
        }
    }
}

void StateManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // StateList::iterator i, j = m_states.end();

    for (unsigned int i = m_topOpaque; i < m_states.size(); i++)
    // for (unsigned int i = m_topOpaque; i < m_states.size(); i++)
    {
        target.draw(*m_states[i], states);
        // State *state = *i;
        // target.draw(*state, states);
    }
}

void StateManager::updateTops()
{
    m_topModal = m_topOpaque = 0;

    for (unsigned int i = m_states.size(); i > 0; i--)
    {
        if ((i > m_topModal) && m_states[i]->isModal())
        {
            m_topModal = i;
        }
        if ((i > m_topOpaque) && m_states[i]->isOpaque())
        {
            m_topOpaque = i;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

