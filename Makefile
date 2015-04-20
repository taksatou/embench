.PHONY: clean
all:: embench

SQUIRREL_BASE = ./vendor/SQUIRREL3
V8_BASE = ./vendor/v8
V8_LDLIBS = -Wl,--start-group \
	$(foreach l, base libbase snapshot libplatform, ${V8_BASE}/out/native/obj.target/tools/gyp/libv8_${l}.a) \
	$(foreach l, uc i18n data, ${V8_BASE}/out/native/obj.target/third_party/icu/libicu${l}.a) \
	-Wl,--end-group
ECL_BASE = ./vendor/ecl15
MRUBY_BASE = ./vendor/mruby-1.1.0

CXX = clang++-3.5
CXXFLAGS = -Wall -std=c++0x -pthread 
CPPFLAGS = -I/usr/include/lua5.1 -I${SQUIRREL_BASE}/include -I${V8_BASE} -I${ECL_BASE}/include -I${MRUBY_BASE}/include
LDFLAGS = -pthread -L${ECL_BASE}/lib -Wl,-rpath=${ECL_BASE}/lib 
LDLIBS = -llua5.1-c++ ${SQUIRREL_BASE}/lib/libsquirrel.a ${SQUIRREL_BASE}/lib/libsqstdlib.a ${V8_LDLIBS} -lecl ${MRUBY_BASE}/build/host/lib/libmruby.a

embench: embench.o lua.o squirrel3.o jsv8.o ecl.o mruby.o
embench.o: embench.cpp
lua.o: lua.cpp
squirrel3.o: squirrel3.cpp
jsv8.o: jsv8.cpp
ecl.o: ecl.cpp
mruby.o: mruby.cpp

clean:
	rm -rf *.o embench

