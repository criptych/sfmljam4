////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "Layer.hpp"

#include "Object.hpp"

////////////////////////////////////////////////////////////////////////////////

Layer::Layer()
{
}

Layer::~Layer()
{
}

void Layer::update(sf::Time delta)
{
    for (Handle<Object> &object : m_objects)
    {
        object->update(delta);
    }

    if (m_following != nullptr)
    {
        /// @todo update position
    }
}

void Layer::follow(Object &object)
{
    m_following = Handle<Object>(object);
}

void Layer::follow()
{
    m_following = Handle<Object>();
}

void Layer::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    for (const Handle<Object> &object : m_objects)
    {
        target.draw(object, states);
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

