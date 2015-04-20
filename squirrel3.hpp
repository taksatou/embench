#ifndef _SQUIRREL3_H_
#define _SQUIRREL3_H_

#include "lang.hpp"


typedef struct SQVM* HSQUIRRELVM;

namespace mojavy {

class Squirrel3 : public Lang {
private:
    HSQUIRRELVM mV;
    
public:
    Squirrel3();
    virtual ~Squirrel3();
    virtual int init();    
    virtual std::string name() const;    
    virtual std::string echo(const std::string &s) const;
    virtual std::map<std::string, std::string> invert(const std::map<std::string, std::string> &lis) const;
};

}


#endif /* _SQUIRREL3_H_ */
