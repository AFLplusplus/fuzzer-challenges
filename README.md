# Fuzzing introspection testcases

Test a fuzzer what fuzzing challenges it can solve.
The goal is to have testcases that afl++ can all complete - once they are all implemented.
Currently the different solving mechanisms have to be set as defined in `src/afl-fuzz-redqueen.c`.
The comparisons to honggfuzz and libfuzzer are biased, as the testcases are how afl-fuzz sees things.
The `libfuzzer/` directories has libfuzzer's applicable own testcases, of which afl++ is only able to solve 64% (vs 86% honggfuzz and 92% libfuzzer).

## Setup

The fuzzers and necessary compilers must be in the path: `afl-clang-fast`, `afl-fuzz`, `hfuzz-clang`, `honggfuzz`, `clang`.

## Running

```
./test.sh TARGET [SPECIFIC_TESTCASE]
```

`TARGET` can be (currently) one of:
  * afl++
  * afl++-qemu
  * honggfuzz
  * libfuzzer
  * kirenenko (via `test-kirenenko.sh`, use it's docker container)
  * symcc + qemu (via `test-symcc.sh` and `test-symqemu.sh`, use it's docker container)
  * manticore (via `test-manticore.sh`)

Note that manitcore and symqemu success can depend on compile options.

## Testcases:

  * `test-u8` - several chained 8 bits checks
  * `test-u16` - several chained 16 bits checks
  * `test-u32` - several chained 32 bits checks
  * `test-u64` - several chained 64 bits checks
  * `test-u128` - several chained 128 bits checks
  * `test-u32-cmp` - several chained 32 bit lesser/greater checks
  * `text-extint` - llvm _ExtInt() tests
  * `test-float` - several chained float checks
  * `test-double` - several chained double checks
  * `test-longdouble` - several chained long double checks
  * `test-memcmp` - several chained memcmp checks
  * `test-strcmp` - several chained strncasecmp checks
  * `test-transform` - different transforming string checks
  * `test-crc32` - several chained crc32 checks

The testcases are WIP - some might be further refined in the future and more added.

## Results

On success the time to solve is displayed.

On failure the generated corpus files are displayed (so you can see how many finds there were). There is also an output log called `TESTCASE.log`.

## Test as of 10th April 2021

All from current repository state (afl++_vX is the -l option value X).
Solve time: 120 seconds for afl++/honggfuzz/libfuzzer
Sym*, Kirenenko and Manticore are not fuzzers but solvers, hence no timing.
Also SymCC and SymQEMU have the same results so they are combined to save space.

|testcase|afl++|kirenenko|sym*|manticore|afl++ QEMU|honggfuzz-2.4|libfuzzer-12|
|:------:|:---:|:-------:|:--:|:-------:|:--------:|:-----------:|:----------:|
|test-crc32|0m5,596s|OK|OK|OK|0m14,609s|FAIL|0m55,987s|
|test-double|0m3,374s|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|
|test-extint|0m1,663s|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|
|test-float|0m1,650s|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|
|test-longdouble|0m1,241s|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|
|test-memcmp|0m0,622s|OK|OK|OK|0m6,494s|0m1,023s|0m1,269s|
|test-strcmp|0m0,623s|FAIL|FAIL|FAIL|0m5,727s|0m1,220s|0m1,469s|
|test-transform|0m4,334s|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|
|test-u128|0m0,418s|FAIL|OK|FAIL|FAIL|FAIL|FAIL|
|test-u16|0m1,233s|OK|OK|OK|0m8,132s|0m1,425s|0m12,065s|
|test-u32|0m1,434s|OK|OK|OK|0m5,185s|0m1,025s|0m6,984s|
|test-u32-cmp|1m29,184s|OK|OK|OK|1m42,470s|0m1,824s|0m0,759s|
|test-u64|0m0,623s|OK|OK|OK|0m3,844s|0m1,024s|0m10,346s|
|test-u8|0m1,449s|OK|OK|OK|0m18,186s|0m1,026s|0m4,382s|

afl++_v3 has the most solves, but due to the many solve attempts overall fuzzing performance is decreased, as can be seen at [https://www.fuzzbench.com/reports/experimental/2021-01-12-aflpp/](https://www.fuzzbench.com/reports/experimental/2021-01-20-aflpp/).
Interpretation: the **slowest** solver is the best in real-world fuzzing.

## More testcases or fuzzers?

Just add them and send a PR.
