#include <iostream>
#include "jsv8.hpp"

using namespace v8;

namespace mojavy {

JS_V8::JS_V8() {
    mIsolate = NULL;
    mPlatform = NULL;
}
    
JS_V8::~JS_V8() {
    if (!mIsolate) return;

    mIsolate->Dispose();
    V8::Dispose();
    V8::ShutdownPlatform();
    delete mPlatform;
}
   
int JS_V8::init() {
    V8::InitializeICU();
    mPlatform = platform::CreateDefaultPlatform();
    V8::InitializePlatform(mPlatform);
    V8::Initialize();
    
    mIsolate = Isolate::New();
    Isolate::Scope isolate_scope(mIsolate);
    HandleScope handle_scope(mIsolate);
    Local<Context> context = Context::New(mIsolate);
    mPersistentContext.Reset(mIsolate, context);
    Context::Scope context_scope(context);

    Local<String> code = String::NewFromUtf8(mIsolate,
                                             "function echo(x) { return x; }\n"
                                             "function invert(x) {\n"
                                             "    var r = {};\n"
                                             "    for (var k in x) {\n"
                                             "        r[x[k]] = k;\n"
                                             "    }\n"
                                             "    return r;\n"
                                             "}\n"
    );
    Local<Script> script = Script::Compile(code);
    script->Run();
    return 0;
}
    
std::string JS_V8::name() const {
    return "v8";
}
    
std::string JS_V8::echo(const std::string &s) const {
    Isolate::Scope isolate_scope(mIsolate);
    HandleScope handle_scope(mIsolate);
    Local<Context> context = Local<Context>::New(mIsolate, mPersistentContext);
    Context::Scope context_scope(context);

    Local<Value> f = context->Global()->Get(String::NewFromUtf8(mIsolate, "echo"));
    if (!f->IsFunction()) {
        return "";
    }
    Local<Value> args[] = {String::NewFromUtf8(mIsolate, s.c_str())};
    Local<Value> res = Function::Cast(*f)->Call(context->Global(), 1, args);
    return *String::Utf8Value(res);
//    return *utf8;
}

std::map<std::string, std::string> JS_V8::invert(const std::map<std::string, std::string> &lis) const {
    std::map<std::string, std::string> res;

    Isolate::Scope isolate_scope(mIsolate);
    HandleScope handle_scope(mIsolate);
    Local<Context> context = Local<Context>::New(mIsolate, mPersistentContext);
    Context::Scope context_scope(context);

    Local<Value> f = context->Global()->Get(String::NewFromUtf8(mIsolate, "invert"));
    if (!f->IsFunction()) {
        return {};
    }

    Local<Object> o = Object::New(mIsolate);
    for (auto it = lis.begin(); it != lis.end(); ++it) {
        o->Set(String::NewFromUtf8(mIsolate, it->first.c_str()), String::NewFromUtf8(mIsolate, it->second.c_str()));
    }
    
    Local<Value> args[] = {o};
    Local<Object> r = Function::Cast(*f)->Call(context->Global(), 1, args)->ToObject(mIsolate);
    Local<Array> keys = r->GetOwnPropertyNames();
    for (int i = 0; i < keys->Length(); ++i) {
        Local<Value> k = keys->Get(i);
        res[*String::Utf8Value(k)] = *String::Utf8Value(r->Get(k));
    }
    return res;
}

}
