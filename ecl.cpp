#include "ecl.hpp"
#include <ecl/ecl.h>

namespace mojavy {

ECL::ECL() {
    char *args[] = {"embench"};
    cl_boot(1, args);
}
    
ECL::~ECL() {
    cl_shutdown();
}
   
int ECL::init() {
    cl_eval(c_string_to_object("(defun echo (x) x)"));
    return 0;
}
    
std::string ECL::name() const {
    return "ecl";
}
    
std::string ECL::echo(const std::string &s) const {
    cl_object o = cl_funcall(2, c_string_to_object("echo"), ecl_make_pointer((void*)s.c_str()));
    return (char*)ecl_to_pointer(o);
}

std::map<std::string, std::string> ECL::invert(const std::map<std::string, std::string> &lis) const {
    return {};
}

}
