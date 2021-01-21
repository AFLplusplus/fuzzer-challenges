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

## Test as of 21th January 2021

All from current repository state (afl++_vX is the -l option value X).
Solve time: 120 seconds

|testcase|afl++_v3|afl++_v2|afl++_v1|afl++_QEMU_v3|honggfuzz-2.3|libfuzzer-12|
|:------:|:------:|:------:|:------:|:-------:|:-----------:|:----------:|
|test-crc32|0m5,596s|0m18,083s|0m38,924s|0m14,609s|FAIL|0m55,987s|
|test-double|0m3,374s|0m2,414s|0m14,064s|FAIL|FAIL|FAIL|
|test-extint|0m1,663s|0m1,051s|0m0,988s|FAIL|FAIL|FAIL|
|test-float|0m1,650s|0m1,206s|0m15,023s|FAIL|FAIL|FAIL|
|test-longdouble|0m1,241s|0m2,651s|0m15,159s|FAIL|FAIL|FAIL|
|test-memcmp|0m0,622s|0m0,835s|0m13,031s|FAIL|0m1,023s|0m1,269s|
|test-strcmp|0m0,623s|0m0,624s|0m9,745s|0m5,727s|0m1,220s|0m1,469s|
|test-transform|0m4,334s|FAIL|FAIL|FAIL|FAIL|FAIL|
|test-u128|0m0,418s|0m0,416s|0m1,272s|FAIL|FAIL|FAIL|
|test-u16|0m1,233s|0m1,442s|0m27,811s|0m8,132s|0m1,425s|0m12,065s|
|test-u32|0m1,434s|0m1,641s|0m2,085s|0m5,185s|0m1,025s|0m6,984s|
|test-u32-cmp|1m29,184s|0m14,712s|0m53,016s|FAIL|0m1,824s|0m0,759s|
|test-u64|0m0,623s|0m0,625s|0m4,063s|0m3,844s|0m1,024s|0m10,346s|
|test-u8|0m1,449s|0m1,259s|0m43,906s|0m18,186s|0m1,026s|0m4,382s|

afl++_v3 has the most solves, but due to the many solve attempts overall fuzzing performance is decreased, as can be seen at [https://www.fuzzbench.com/reports/experimental/2021-01-12-aflpp/](https://www.fuzzbench.com/reports/experimental/2021-01-20-aflpp/).
Interpretation: the **slowest** solver is the best in real-world fuzzing.

## More testcases or fuzzers?

Just add them and send PR.
