#include "mruby.hpp"
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/string.h>
#include <mruby/hash.h>
#include <mruby/array.h>
#include <mruby/value.h>
#include <iostream>

namespace mojavy {

MRuby::MRuby() {
    mMrb = mrb_open();
    mContext = mrbc_context_new(mMrb);
}
    
MRuby::~MRuby() {
   mrbc_context_free(mMrb, mContext);
   mrb_close(mMrb);
}
   
int MRuby::init() {
    const char *code =
        "def echo(x); x; end;"
        "def invert(x)\n"
        "  r = {}\n"
        "  x.each do |k, v|\n"
        "    r[v] = k\n"
        "  end\n"
        "  r\n"
        "end\n"
        ;
        
    struct mrb_parser_state *p = mrb_parse_string(mMrb, code, mContext);
    if (p->nerr > 0) {
        std::cerr << "parse error" << std::endl;
        mrb_parser_free(p);
        return 1;
    }
        
    struct RProc *proc = mrb_generate_code(mMrb, p);
    mrb_parser_free(p);
    if (proc == NULL) {
        std::cerr << "compile error" << std::endl;
        return 1;
    }

    mrb_run(mMrb, proc, mrb_top_self(mMrb));
    return 0;
}
    
std::string MRuby::name() const {
    return "mruby";
}
    
std::string MRuby::echo(const std::string &s) const {
    mrb_value a = mrb_str_new_cstr(mMrb, s.c_str());
    mrb_value r = mrb_funcall(mMrb, mrb_top_self(mMrb), "echo", 1, a);
    return mrb_str_to_cstr(mMrb, r);
}

std::map<std::string, std::string> MRuby::invert(const std::map<std::string, std::string> &lis) const {
    std::map<std::string, std::string> res;

    mrb_value a = mrb_hash_new_capa(mMrb, lis.size());
    for (auto it = lis.begin(); it != lis.end(); ++it) {
        mrb_hash_set(mMrb, a, mrb_str_new_cstr(mMrb, it->first.c_str()), mrb_str_new_cstr(mMrb, it->second.c_str()));
    }
    mrb_value r = mrb_funcall(mMrb, mrb_top_self(mMrb), "invert", 1, a);
    mrb_value keys = mrb_hash_keys(mMrb, r);
    for (int i = 0; i < RARRAY_LEN(keys); ++i) {
        mrb_value k = mrb_ary_ref(mMrb, keys, i);
        mrb_value v = mrb_hash_get(mMrb, r, k);
        res[mrb_str_to_cstr(mMrb, k)] = mrb_str_to_cstr(mMrb, v);
    }
    return res;
}

}
