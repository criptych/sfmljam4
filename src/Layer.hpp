////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __LAYER_HPP__
#define __LAYER_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include <SFML/Graphics.hpp>

////////////////////////////////////////////////////////////////////////////////

class Layer : public sf::Drawable
{
public:
    Layer();
    ~Layer();

    void update(sf::Time delta);

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
};

////////////////////////////////////////////////////////////////////////////////

#endif // __LAYER_HPP__

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

