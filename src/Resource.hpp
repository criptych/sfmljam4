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
    Handle():
        m_t()
    {
    }

    template <typename X>
    Handle(X *x):
        m_t()
    {
        set(x);
    }

    template <typename X>
    Handle(const Handle<X> &h)
    {
        set(h.operator X*());
    }

    template <typename X>
    Handle(Handle<X> &&h)
    {
        T *p = m_t;
        m_t = h.m_t;
        h.m_t = p;
    }

    ~Handle()
    {
        if (m_t != nullptr)
        {
            m_t->release();
            m_t = nullptr;
        }
    }

    template <typename X>
    Handle<T> &operator=(X *x)
    {
        set(x);
        return *this;
    }

    template <typename X>
    Handle<T> &operator=(const Handle<X> &h)
    {
        set(h.operator X*());
        return *this;
    }

    template <typename X>
    bool operator==(const Handle<X> &h) const
    {
        return m_t == h.operator X*();
    }

    template <typename X>
    bool operator==(const X *x) const
    {
        return m_t == x;
    }

    operator T&() const
    {
        return *m_t;
    }

    operator T*() const
    {
        return m_t;
    }

    T *operator->() const
    {
        return m_t;
    }

private:
    template <typename X>
    void set(X *x)
    {
        if (m_t != x)
        {
            T *t = m_t;

            if (x != nullptr)
            {
                x->acquire();
            }

            m_t = x;

            if (t != nullptr)
            {
                t->release();
            }
        }
    }

    T *m_t;
};

////////////////////////////////////////////////////////////////////////////////

#endif // __RESOURCE_HPP__

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

