#ifndef _LANG_H_
#define _LANG_H_

#include <string>
#include <map>

namespace mojavy {

class Lang {
public:
    virtual ~Lang() {};
    virtual int init() = 0;
    virtual std::string name() const = 0;
    virtual std::string echo(const std::string &s) const = 0;
    virtual std::map<std::string, std::string> invert(const std::map<std::string, std::string> &lis) const = 0;
};

class Dumb : public Lang {
private:
    
public:
    Dumb() {}
    virtual ~Dumb() {}
   
    virtual int init() {
        return 0;
    }
    
    virtual std::string name() const {
        return "dumb";
    }
    
    virtual std::string echo(const std::string &s) const {
        return std::string(s);
    }

    virtual std::map<std::string, std::string> invert(const std::map<std::string, std::string> &lis) const {
        std::map<std::string, std::string> r;
        for (auto it = lis.begin(); it != lis.end(); ++it) {
            r[it->second] = it->first;
        }
        return r;
    }

};


}

#endif /* _LANG_H_ */
