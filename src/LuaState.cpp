////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "LuaState.hpp"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
};

#include <utility>

////////////////////////////////////////////////////////////////////////////////

LuaState::LuaState(lua_State *lua):
    m_lua(lua)
{
}

LuaState::LuaState(LuaState &&rhs):
    m_lua(nullptr)
{
    std::swap(m_lua, rhs.m_lua);
}

LuaState::LuaState():
    m_lua(luaL_newstate())
{
    if (!m_lua)
    {
        throw "Failed creating Lua state";
    }
}

LuaState::~LuaState()
{
    if (m_lua != nullptr)
    {
        if (lua_pushthread(m_lua))
        {
            lua_close(m_lua);
        }
        else
        {
            lua_pop(m_lua, 1); // pop thread to keep the stack unchanged
        }

        m_lua = nullptr;
    }
}

LuaState::operator lua_State * ()
{
    return m_lua;
}

LuaState::operator const lua_State * () const
{
    return m_lua;
}

void LuaState::loadStdLibs()
{
    luaL_openlibs(m_lua);
}

void LuaState::loadSafeLibs()
{
    ////////////////////////////////////////////////////////////
    // SAFE libraries (no unsafe system access):
    //

    static const luaL_Reg safe_libs[] = {
      {"_G",            luaopen_base},
      {LUA_TABLIBNAME,  luaopen_table},
      {LUA_STRLIBNAME,  luaopen_string},
      {LUA_MATHLIBNAME, luaopen_math},
    #if LUA_VERSION_NUM >= 502
      {LUA_COLIBNAME,   luaopen_coroutine}, // was part of base lib until 5.2
      {LUA_BITLIBNAME,  luaopen_bit32},     // nb: deprecated in 5.3
    #endif
    #if LUA_VERSION_NUM >= 503
      {LUA_UTF8LIBNAME, luaopen_utf8},
    #endif
      {NULL, NULL}
    };

    for (const luaL_Reg *lib = safe_libs; lib->func; lib++)
    {
        lua_pushcfunction(m_lua, lib->func);
        lua_pushstring(m_lua, lib->name);
        lua_call(m_lua, 1, 0);
    }

    ////////////////////////////////////////////////////////////
    // UNSAFE libraries (potentially unrestricted system access):
    //
    // luaopen_package  // 'require' may load unsafe modules
    // luaopen_io       // unrestricted disk I/O
    // luaopen_os       // unrestricted OS operations
    // luaopen_debug    // "back-door" access to internals
    //

    ////////////////////////////////////////////////////////////
    // NOTE that even the SAFE libraries do not prevent user
    // scripts from causing e.g. deadlock or infinite loops, and
    // the base library does allow loading pure Lua modules
    // using `load*' and `dofile'.
    //
}

void LuaState::doString(const char *string)
{
    if (luaL_dostring(m_lua, string))
    {
        throw Error(lua_tostring(m_lua, -1));
    }
}

void LuaState::doFile(const char *filename)
{
    if (luaL_dofile(m_lua, filename))
    {
        throw Error(lua_tostring(m_lua, -1));
    }
}

const char *LuaState::toString(int index)
{
    return lua_tostring(m_lua, index);
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

