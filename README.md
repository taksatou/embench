## embench

Benchmarks of embedded scripting languages.

Currently, following languages are implemented.

* lua
* squirrel
* V8
* mruby
* Embeddable Common-Lisp

To be as fair as possible, all embedded languages are called from the same interface. All functions are compile and registered on the global scope of the target language at the initialization phase.

This benchmark focuses on performance of the embedding operations, rather than the language performance itself.


## result

environment:

* Linux 3.2.0-67
* Intel(R) Core(TM) i7-2600S CPU @ 2.80GHz

### ./embench echo 100000


| lang | msec |
| ---- | ---- |
| (control) | 2.134 |
| Lua |     15.164 |
| LuaJIT |   12.531 |
| Squirrel |      22.273 |
| V8 |      100.93 |
| mruby |   36.118 |
| ecl |     288.792 |

### ./embench invert 100000

| lang | msec |
| ---- | ---- |
| (control) |    183.136 |
| Lua | 505.398 |
| LuaJIT |     415.429 |
| Squirrel |      480.025 |
| V8 |      1033.16 |
| mruby |   1260.61 |
| ecl |     - |


