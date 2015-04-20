#ifndef _ECL_H_
#define _ECL_H_

#include "lang.hpp"

namespace mojavy {

class ECL : public Lang {
private:
//     *mL;
    
public:
    ECL();
    virtual ~ECL();
    virtual int init();    
    virtual std::string name() const;    
    virtual std::string echo(const std::string &s) const;
    virtual std::map<std::string, std::string> invert(const std::map<std::string, std::string> &lis) const;
};

}


#endif /* _ECL_H_ */
