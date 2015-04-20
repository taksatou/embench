#include "squirrel3.hpp"
#include <squirrel.h>
#include <sqstdblob.h>
#include <sqstdsystem.h>
#include <sqstdio.h>
#include <sqstdmath.h>
#include <sqstdstring.h>
#include <sqstdaux.h>

#include <iostream>
#include <string>
#include <cstdio>

namespace mojavy {

void _compile_error(HSQUIRRELVM v, const SQChar *desc, const SQChar *source, SQInteger line, SQInteger column) {
    char buf[1024];
    int len = snprintf(buf, 1024, "%s:%lld:%lld\t %s", source, line, column, desc);
    sq_pushstring(v, buf, len);
}

Squirrel3::Squirrel3() {
    mV = sq_open(1024);
}

Squirrel3::~Squirrel3() {
    sq_close(mV);
}

int Squirrel3::init() {
    const static char code[] =
        "function echo(x) { return x; }\n"
        "function invert(x) {\n"
        "    local r = {};\n"
        "    foreach (k, v in x) {\n"
        "        r[v] <- k;\n"
        "    }\n"
        "    return r;\n"
        "}\n"
        // "function err(env, self) { print(env); }\n"
        // "seterrorhandler(err);\n"
    ;

    sq_pushroottable(mV);

    sq_setcompilererrorhandler(mV, _compile_error);
    if (SQ_FAILED(sq_compilebuffer(mV, code, sizeof(code), "embench", SQTrue))) {
        const SQChar *s;
        sq_getstring(mV, -1, &s);
        std::cerr << s << std::endl;
        sq_poptop(mV);
        return 1;
    }

    sq_pushroottable(mV);
    // execute the compiled code
    if (SQ_FAILED(sq_call(mV, 1, SQFalse, SQTrue))) {
        return 1;
    }

    // register libraries
    sqstd_register_systemlib(mV);
    sqstd_register_iolib(mV);

    sq_poptop(mV);

    return 0;
}

std::string Squirrel3::name() const {
    return "sq";
}

std::string Squirrel3::echo(const std::string &s) const {
//    std::cerr << sq_gettop(mV) << std::endl;

    SQInteger top = sq_gettop(mV);
    sq_pushroottable(mV);
    sq_pushstring(mV, "echo", -1);
    sq_get(mV, -2);
    sq_pushroottable(mV);
    sq_pushstring(mV, s.c_str(), s.size());
    if (SQ_FAILED(sq_call(mV, 2, SQTrue, SQTrue))) {
        std::cerr << "failed to call echo" << std::endl;
        return "";
    }

    const SQChar *tmp;
    sq_getstring(mV, -1, &tmp);

    std::string res(tmp);
    sq_settop(mV, top);
    return res;
}

std::map<std::string, std::string> Squirrel3::invert(const std::map<std::string, std::string> &lis) const {
    std::map<std::string, std::string> res;

    SQInteger top = sq_gettop(mV);
    sq_pushroottable(mV);

    sq_pushstring(mV, "invert", -1);
    sq_get(mV, -2);
    sq_pushroottable(mV);
    sq_newtable(mV);
    for (auto it = lis.begin(); it != lis.end(); ++it) {
        sq_pushstring(mV, it->first.c_str(), it->first.size());
        sq_pushstring(mV, it->second.c_str(), it->second.size());
        sq_newslot(mV, -3, SQFalse);
    }
    if (SQ_FAILED(sq_call(mV, 2, SQTrue, SQTrue))) {
        std::cerr << "failed to call invert" << std::endl;
        return {};
    }

    sq_pushnull(mV);
    while (SQ_SUCCEEDED(sq_next(mV, -2))) {
        const SQChar *k;
        const SQChar *v;
        sq_getstring(mV, -2, &k);
        sq_getstring(mV, -1, &v);
        res[k] = v;
        sq_pop(mV, 2);
    }

    sq_settop(mV, top);
    return res;
}

}
