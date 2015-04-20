#ifndef _MRUBY_H_
#define _MRUBY_H_

#include "lang.hpp"

struct mrb_state;
struct mrbc_context;

namespace mojavy {

class MRuby : public Lang {
private:
    struct mrb_state *mMrb;
    struct mrbc_context *mContext;
    
public:
    MRuby();
    virtual ~MRuby();
    virtual int init();    
    virtual std::string name() const;    
    virtual std::string echo(const std::string &s) const;
    virtual std::map<std::string, std::string> invert(const std::map<std::string, std::string> &lis) const;
};

}


#endif /* _MRUBY_H_ */
