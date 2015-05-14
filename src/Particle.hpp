////////////////////////////////////////////////////////////////////////////////
// Particle systems
//
// Based somewhat on MOAI particle system classes
////////////////////////////////////////////////////////////////////////////////

#ifndef __PARTICLE_HPP__
#define __PARTICLE_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include "Resource.hpp"

#include <SFML/Graphics.hpp>

////////////////////////////////////////////////////////////////////////////////

class Particle
{
public:
    class Force : public Resource
    {
    public:
        enum Type
        {
            Linear,
            Radial,
            Attractor,
            Basin,
        };

    public:
        Force(Type type, const sf::Vector2f &vector = sf::Vector2f(), float radius = 0.f);

        void setType(Type type);
        Type getType() const;

        void setVector(const sf::Vector2f &vector);
        const sf::Vector2f &getVector() const;

        void setRadius(float radius);
        float getRadius() const;

        void apply(sf::Vector2f &position, sf::Vector2f &acceleration, float mass) const;

    private:
        Type m_type;
        sf::Vector2f m_vector;
        float m_radius;
    };

    class State : public Resource
    {
    public:
        State(float mass = 1.f, State *next = nullptr);

        void addForce(const Particle::Force &force);
        Particle::Force &getForce(unsigned int i);
        Particle::Force &removeForce(unsigned int i);

    private:
        std::vector< Handle<Particle::Force> > m_forces;
        float m_mass;
        State *m_next;
    };

    class System : public sf::Drawable
    {
    public:
        System();
        ~System();

        void start();
        void stop();

        bool isRunning() const;

        void update(sf::Time delta);

        void addState(const Particle::State &force);
        Particle::State &getState(unsigned int i);
        Particle::State &removeState(unsigned int i);

    protected:
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    private:
        bool m_running;
        std::vector<Particle> m_particles;
        std::vector< Handle<Particle::State> > m_states;
    };

protected:
    Particle();

public:
    ~Particle();

private:
    sf::Vector2f m_position;
    sf::Vector2f m_scale;
    sf::Vector2f m_velocity;
    sf::Color m_color;
    float m_rotation;
    float m_spin;
    float m_age;
    float m_life;
    Handle<State> m_state;
};

////////////////////////////////////////////////////////////////////////////////

#endif // __PARTICLE_HPP__

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

