#ifndef _JS_V8_H_
#define _JS_V8_H_

#include "lang.hpp"
#include <include/v8.h>
#include <include/libplatform/libplatform.h>

namespace mojavy {

class JS_V8 : public Lang {
private:
    v8::Isolate *mIsolate;
    v8::Platform *mPlatform;
    v8::Persistent<v8::Context> mPersistentContext;
    
public:
    JS_V8();
    virtual ~JS_V8();
    virtual int init();    
    virtual std::string name() const;    
    virtual std::string echo(const std::string &s) const;
    virtual std::map<std::string, std::string> invert(const std::map<std::string, std::string> &lis) const;
};

}


#endif /* _JS_V8_H_ */
