////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include "Resource.hpp"

#include <SFML/Graphics.hpp>

////////////////////////////////////////////////////////////////////////////////

class Object : public sf::Drawable, public sf::Transformable, public Resource
{
public:
    Object();
    ~Object();

    void update(sf::Time delta);

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
};

////////////////////////////////////////////////////////////////////////////////

#endif // __OBJECT_HPP__

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

