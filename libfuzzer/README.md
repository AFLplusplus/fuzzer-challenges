# libfuzzer testcases

This is an import and transformation from the llvm libfuzzer-12 unit tests
that can be used with different fuzzers than libfuzzer..

They can be run by executing `../test.sh` while being in this subdirectory.

Current results:
```
AFL++: SUCCESS=23 FAIL=13
HONGGFUZZ: SUCCESS=31 FAIL=5
LIBFUZZER: SUCCESS=33 FAIL=3
```
