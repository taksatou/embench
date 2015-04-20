#include "lua.hpp"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <iostream>

namespace mojavy {

Lua::Lua() {
    mL = lua_open();
}
    
Lua::~Lua() {
    lua_close(mL);
}
   
int Lua::init() {
    luaL_openlibs(mL);
    return luaL_dostring(mL,
                         "function echo(x)\n"
                         "  return x\n"
                         "end\n"
                         "function invert(x)\n"
                         "   local r = {}\n"
                         "   for k, v in pairs(x) do\n"
//                         " print(k, v)\n"
                         "      r[v] = k\n"
                         "   end\n"
                         "   return x\n"
                         "end\n"
    );
}
    
std::string Lua::name() const {
    return "lua";
}
    
std::string Lua::echo(const std::string &s) const {
    lua_getglobal(mL, "echo");
    lua_pushlstring(mL, s.c_str(), s.size());
    if (lua_pcall(mL, 1, 1, 0) != 0) {
        luaL_error(mL, "failed to call echo: %s", lua_tostring(mL, -1));
    }
    
    std::string res(lua_tostring(mL, -1));
    lua_pop(mL, 1);
    return res;
}

std::map<std::string, std::string> Lua::invert(const std::map<std::string, std::string> &lis) const {
    std::map<std::string, std::string> res;

    lua_getglobal(mL, "invert");
    lua_newtable(mL);
    for (auto it = lis.begin(); it != lis.end(); ++it) {
        lua_pushlstring(mL, it->first.c_str(), it->first.size());
        lua_pushlstring(mL, it->second.c_str(), it->second.size());
        lua_settable(mL, -3);
    }

    if (lua_pcall(mL, 1, 1, 0) != 0) {
        luaL_error(mL, "failed to call invert: %s", lua_tostring(mL, -1));
    }

    lua_pushnil(mL);
    while (lua_next(mL, 1) != 0) {
        res[lua_tostring(mL, -1)] = lua_tostring(mL, -2);
        lua_pop(mL, 1);
    }

    lua_pop(mL, 1);
    return res;
}

}
