# Fuzzing introspection testcases

Test a fuzzer what fuzzing challenges it can solve.

## Setup

The fuzzer and necessary compilers must be in the path: `afl-clang-fast`, `afl-fuzz`, `hfuzz-clang`, `honggfuzz`, `clang`.

## Running

```
./test.sh
```

`TARGET` can be (currently) one of:
  * afl++
  * honggfuzz
  * libfuzzer

## Testcases:

  * `test-u8` - several chained 8 bits checks
  * `test-u16` - several chained 16 bits checks
  * `test-u32` - several chained 32 bits checks
  * `test-u64` - several chained 64 bits checks
  * `test-u128` - several chained 128 bits checks
  * `test-u32-cmp` - several chained 32 bit lesser/greater checks
  * `test-float` - several chained float checks
  * `test-double` - several chained double checks
  * `test-memcmp` - several chained memcmp checks
  * `test-strcmp` - several chained strncasecmp checks
  * `test-crc32` - several chained crc32 checks

## Results

All fuzzers fail one or more checks (e.g. no fuzzer can solve the 128 bits challenge).

On success the time to solve is displayed (note that afl++ has a longer startup time than all other fuzzers).

On failure the the generated corpus files are displayed (so you can see how many finds there were). There is also an output log called `TESTCASE.log`.

## More testcases or fuzzers?

Just add them and send PR.

