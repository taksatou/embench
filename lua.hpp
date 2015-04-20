#ifndef _LUA_H_
#define _LUA_H_

#include "lang.hpp"

typedef struct lua_State lua_State;

namespace mojavy {

class Lua : public Lang {
private:
    lua_State *mL;
    
public:
    Lua();
    virtual ~Lua();
    virtual int init();    
    virtual std::string name() const;    
    virtual std::string echo(const std::string &s) const;
    virtual std::map<std::string, std::string> invert(const std::map<std::string, std::string> &lis) const;
};

}


#endif /* _LUA_H_ */
