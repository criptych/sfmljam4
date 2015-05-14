////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "Particle.hpp"

////////////////////////////////////////////////////////////////////////////////

Particle::Force::Force(Particle::Force::Type type, const sf::Vector2f &vector, float radius):
    m_type(type),
    m_vector(vector),
    m_radius(radius)
{
}

void Particle::Force::setType(Particle::Force::Type type)
{
    m_type = type;
}

Particle::Force::Type Particle::Force::getType() const
{
    return m_type;
}

void Particle::Force::setVector(const sf::Vector2f &vector)
{
    m_vector = vector;
}

const sf::Vector2f &Particle::Force::getVector() const
{
    return m_vector;
}

void Particle::Force::setRadius(float radius)
{
    m_radius = radius;
}

float Particle::Force::getRadius() const
{
    return m_radius;
}

void Particle::Force::apply(sf::Vector2f &position, sf::Vector2f &acceleration, float mass) const
{
    if (mass > 0.f)
    {
        sf::Vector2f force;

        switch (m_type)
        {
            case Type::Linear:
            {
                force = m_vector;
                break;
            }

            case Type::Radial:
            {
                force = m_vector - position;
                float dot = force.x * force.x + force.y * force.y;
                dot = (dot == 0.f) ? 0.f : (1.f / std::sqrt(dot));
                force *= dot;
                break;
            }

            case Type::Attractor:
            {
                force = m_vector - position;
                float dot = force.x * force.x + force.y * force.y;
                dot = (dot == 0.f) ? 0.f : (1.f / std::sqrt(dot));
                force /= dot * dot;
                break;
            }

            case Type::Basin:
            {
                force = m_vector - position;
                float dot = force.x * force.x + force.y * force.y;
                dot = (dot == 0.f) ? 0.f : (1.f / std::sqrt(dot));
                dot /= m_radius;
                force *= dot * dot;
                break;
            }
        }

        acceleration += force / mass;
    }
}

////////////////////////////////////////////////////////////////////////////////

Particle::State::State(float mass, Particle::State *next):
    m_mass(mass),
    m_next(next)
{
}

////////////////////////////////////////////////////////////////////////////////

Particle::System::System()
{
}

Particle::System::~System()
{
}

void Particle::System::update(sf::Time delta)
{
}

void Particle::System::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
}

////////////////////////////////////////////////////////////////////////////////

Particle::Particle()
{
}

Particle::~Particle()
{
    if (m_state)
    {
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

