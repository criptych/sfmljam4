////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __STATE_HPP__
#define __STATE_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include <SFML/Graphics.hpp>

////////////////////////////////////////////////////////////////////////////////

class State : public sf::Drawable
{
protected:
    State();

public:
    virtual ~State();

    bool handleEvent(const sf::Event &event);
    void update(sf::Time delta);

    void exit();

    void pop();

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

public:
    virtual bool isOpaque() const;
    virtual bool isModal() const;

protected:
    virtual void onEnter();
    virtual void onSuspend();
    virtual void onResume();
    virtual void onExit();

    virtual bool onEvent(const sf::Event &event);
    virtual void onUpdate(sf::Time delta);

private:
    friend class StateManager;

    class StateManager *m_mgr;
};

////////////////////////////////////////////////////////////////////////////////

#endif // __STATE_HPP__

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

