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
        state->m_mgr->popState(state);
    }

    m_states.push_back(state);

    sf::err() << "StateManager<" << this << ">::pushState(@" << state << ") [" << m_states.size() << "]\n";

    state->m_mgr = this;

    state->onEnter();
    state->onResume();
}

State *StateManager::popState()
{
    State *last;

    if (!m_states.empty())
    {
        last = m_states.back();

        last->onSuspend();
        last->onExit();
        m_states.pop_back();
        last->m_mgr = nullptr;

        sf::err() << "StateManager<" << this << ">::popState(@" << last << ") [" << m_states.size() << "]\n";

        if (!m_states.empty())
        {
            m_states.back()->onResume();
        }

        updateTops();
    }

    return last;
}

void StateManager::popState(State &state)
{
    return popState(&state);
}

void StateManager::popState(State *state)
{
    StateList::iterator end = m_states.end();

    if (std::remove(m_states.begin(), end, state) != end)
    {
        sf::err() << "StateManager<" << this << ">::popState(@" << state << ") [" << m_states.size() << "]\n";

        if (state->m_mgr == this)
        {
            state->m_mgr = nullptr;
            state->onSuspend();
            state->onExit();
        }

        updateTops();
    }
}

State *StateManager::setState(State &state)
{
    return setState(&state);
}

State *StateManager::setState(State *state)
{
    State *last = popState();
    pushState(state);
    return last;
}

State *StateManager::getState() const
{
    State *top;

    if (!m_states.empty())
    {
        top = m_states.back();
    }

    return top;
}

bool StateManager::handleEvent(const sf::Event &event)
{
    bool handled = false;
    std::find_if(m_states.rbegin(), m_states.rend() - m_topModal,
        [&](State *state) {
            return handled |= state->handleEvent(event);
        });
    return handled;
}

void StateManager::update(sf::Time delta)
{
    std::for_each(m_states.rbegin(), m_states.rend() - m_topModal,
        [&](State *state) {
            state->update(delta);
        });
}

void StateManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    std::for_each(m_states.rbegin(), m_states.rend() - m_topOpaque,
        [&](State *state) {
            state->draw(target, states);
        });
}

void StateManager::updateTops()
{
    m_topModal = m_topOpaque = 0;

    unsigned int i = m_states.size();

    for (State *state : m_states)
    {
        --i;
        if ((i > m_topModal) && state->isModal())
        {
            m_topModal = i;
        }
        if ((i > m_topOpaque) && state->isOpaque())
        {
            m_topOpaque = i;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

