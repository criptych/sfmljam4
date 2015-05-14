////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __RESOURCE_HPP__
#define __RESOURCE_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include <SFML/Config.hpp>

////////////////////////////////////////////////////////////////////////////////

class Resource
{
protected:
    Resource();

public:
    virtual ~Resource();

    void acquire();
    void release();

private:
    sf::Uint32 m_count;
};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class Handle
{
public:
    Handle(T *t = nullptr):
        m_t()
    {
        set(t);
    }

    Handle(const Handle<T> &h)
    {
        set(h);
    }

    Handle(Handle<T> &&h)
    {
        h.set(set(h));
    }

    ~Handle()
    {
        set(nullptr);
    }

    Handle<T> &operator=(T *t)
    {
        set(t);
        return *this;
    }

    operator T*()
    {
        return m_t;
    }

    T *operator->()
    {
        return m_t;
    }

private:
    T *set(T *t)
    {
        if (m_t != t)
        {
            T *x = m_t;

            if (t)
            {
                t->acquire();
            }

            m_t = t;

            if (x)
            {
                x->release();
            }

            return x;
        }

        return t;
    }

    T *m_t;
};

////////////////////////////////////////////////////////////////////////////////

#endif // __RESOURCE_HPP__

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

