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
    StateList::iterator end = m_states.end();

    if (std::remove(m_states.begin(), end, state) != end)
    {
        sf::err() << "StateManager::popState(State&)\n";

        if (state->m_mgr == this)
        {
            state->m_mgr = nullptr;
            state->onSuspend();
            state->onExit();
        }

        updateTops();
    }
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
    bool handled = false;
    std::find_if(m_states.rbegin(), m_states.rend() - m_topModal,
        [&](const Handle<State> &h) {
            return handled |= h->handleEvent(event);
        });
    return handled;
}

void StateManager::update(sf::Time delta)
{
    std::for_each(m_states.rbegin(), m_states.rend() - m_topModal,
        [&](Handle<State> &h) {
            h->update(delta);
        });
}

void StateManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    std::for_each(m_states.rbegin(), m_states.rend() - m_topOpaque,
        [&](const Handle<State> &h) {
            target.draw(h, states);
        });
}

void StateManager::updateTops()
{
    m_topModal = m_topOpaque = 0;

    unsigned int i = m_states.size();

    for (Handle<State> &h : m_states)
    {
        --i;
        if ((i > m_topModal) && h->isModal())
        {
            m_topModal = i;
        }
        if ((i > m_topOpaque) && h->isOpaque())
        {
            m_topOpaque = i;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

