////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __STATEMANAGER_HPP__
#define __STATEMANAGER_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include <SFML/Graphics.hpp>

////////////////////////////////////////////////////////////////////////////////

class State;

class StateManager
{
public:
    StateManager();
    ~StateManager();

    void pushState(State *state);
    void pushState(State &state);
    State *popState();
    void popState(State *state);
    void popState(State &state);
    void removeState(State *state);
    void removeState(State &state);
    State *setState(State *state);
    State *setState(State &state);
    State *getState() const;

    bool handleEvent(const sf::Event &event);
    void update(sf::Time delta);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    void updateTops();

    typedef std::vector<State *> StateList;

    StateList m_states;
    unsigned int m_topModal;
    unsigned int m_topOpaque;
};

////////////////////////////////////////////////////////////////////////////////

#endif // __STATEMANAGER_HPP__

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

