////////////////////////////////////////////////////////////////////////////////
// includes.cpp
//
// Includes all header files in the project, to ensure that all are present and
// compileable before trying to use them in the actual source (i.e. push all
// "could not include header" errors to the front of the line).
//
////////////////////////////////////////////////////////////////////////////////

#include "PhysicsFS.hpp"
#include "PhysInputStream.hpp"
#include "PhysOutputStream.hpp"
#include "LuaState.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <physfs.h>

#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_rect_pack.h>
#include <stb_truetype.h>
#include <stb_perlin.h>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

