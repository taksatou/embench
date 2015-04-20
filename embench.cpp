#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <map>
#include <iostream>
#include <sys/time.h>
#include "lua.hpp"
#include "squirrel3.hpp"
#include "jsv8.hpp"
#include "ecl.hpp"
#include "mruby.hpp"

using namespace std;
using namespace mojavy;

typedef void (*bench_fn)(Lang *l, int num);

void test(Lang *l, int num) {
    {  // test echo
        string s = "abc";
        string t = l->echo(s);
        if (s != t) {
            cerr << "echo: " << l->name() << " is not working. input: " << s << ", output: " << t << endl;
        }
    }

    {  // test invert
        map<string, string> lis = {
            {"a", "A"},
            {"b", "B"},
            {"c", "C"},
            {"d", "D"},
            {"e", "E"},
        };

        auto r = l->invert(lis);
        for (auto it = lis.begin(); it != lis.end(); ++it) {
            if (r[it->second] != it->first) {
                cerr << "invert: " << l->name() << " is not working" << endl;
                break;
            }
        }
    }
}
 
void bench_echo(Lang *l, int num) {
    struct timeval a, b;
    string s = "x";
    
    gettimeofday(&a, NULL);
    for (int i = 0; i < num; i++) {
        l->echo(s);
    }
    gettimeofday(&b, NULL);

    double msec = (b.tv_sec - a.tv_sec) * 1000.0 + (b.tv_usec - a.tv_usec) / 1000.0;
    cout << l->name() << "\t" << msec << endl;
}

void bench_invert(Lang *l, int num) {
    struct timeval a, b;
    map<string, string> lis = {
        {"a", "A"},
        {"b", "B"},
        {"c", "C"},
        {"d", "D"},
        {"e", "E"},
        // {"f", "F"},
        // {"g", "G"},
        // {"h", "H"},
        // {"i", "I"},
        // {"j", "J"},
    };

    gettimeofday(&a, NULL);
    for (int i = 0; i < num; i++) {
        l->invert(lis);
    }
    gettimeofday(&b, NULL);

    double msec = (b.tv_sec - a.tv_sec) * 1000.0 + (b.tv_usec - a.tv_usec) / 1000.0;
    cout << l->name() << "\t" << msec << endl;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "usage: " << argv[0] << " <echo|invert|test> <num>" << endl;
        return 1;
    }
    
    bench_fn fn;
    if (strcmp(argv[1], "echo") == 0) {
        fn = bench_echo;
    } else if (strcmp(argv[1], "invert") == 0) {
        fn = bench_invert;
    } else if (strcmp(argv[1], "test") == 0) {
        fn = test;
    } else {
        cerr << "unknown command" << endl;
        return 1;
    }
    
    int num = std::atoi(argv[2]);
    Lang *lis[] = {
        new Dumb(),
        new Lua(),
        new Squirrel3(),
        new JS_V8(),
        new ECL(),
        new MRuby(),
        NULL,
    };

    for (int i = 0; lis[i] != NULL; ++i) {
        if (lis[i]->init()) {
            cout << "failed to init: " << lis[i]->name() << endl;
        }
    }

    for (int i = 0; lis[i] != NULL; ++i) {
        fn(lis[i], num);
    }

    for (int i = 0; lis[i] != NULL; ++i) {
        delete lis[i];
    }

    return 0;
}
