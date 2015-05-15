////////////////////////////////////////////////////////////////////////////////
// LuaState.hpp
//
// Provides a wrapper around the Lua C API with RAII semantics.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __LUASTATE_HPP__
#define __LUASTATE_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include <exception>
#include <string>

////////////////////////////////////////////////////////////////////////////////

typedef int (*lua_CFunction)(struct lua_State*);

class LuaState
{
public:
    class Error : public std::exception
    {
    public:
        Error(const char *msg): m_msg(msg) {}

        const char *what() const throw() { return m_msg.c_str(); }

    private:
        std::string m_msg;
    };

private:
    LuaState(struct lua_State *lua);

public:
    LuaState(LuaState &&rhs);

    LuaState();
    ~LuaState();

    operator struct lua_State * ();
    operator const struct lua_State * () const;

    void require(const char *name, lua_CFunction func, bool global = false);
    void loadStdLibs();
    void loadSafeLibs();

    bool tryDoString(const char *string);
    bool tryDoFile(const char *filename);

    void doString(const char *string);
    void doFile(const char *filename);

    const char *toString(int index = -1);

    LuaState newState();

private:
    struct lua_State *m_lua;
};

////////////////////////////////////////////////////////////////////////////////

#endif // __LUASTATE_HPP__

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

