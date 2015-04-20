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


