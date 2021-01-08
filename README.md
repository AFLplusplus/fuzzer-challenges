# Fuzzing introspection testcases

Test a fuzzer what fuzzing challenges it can solve.

## Setup

The fuzzers and necessary compilers must be in the path: `afl-clang-fast`, `afl-fuzz`, `hfuzz-clang`, `honggfuzz`, `clang`.

## Running

```
./test.sh TARGET
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

## Test as of 8th January 2021

All from current repository state.
Solve time: 20 seconds (to give libfuzzer a better chance)

|testcase|afl++_variant1|afl++_variant2|afl++_dev|afl++_stable|honggfuzz-2.3|libfuzzer-12|
|:------:|:------------------:|:-------------------:|:-------:|:----------:|:-----------:|:----------:|
|test-crc32|0m5,542s|0m5,304s|0m4,021s|0m4,664s|FAIL|0m2,135s|
|test-double|0m13,603s|0m2,332s|0m1,676s|FAIL|FAIL|FAIL|
|test-float|0m9,856s|0m5,161s|0m5,224s|FAIL|FAIL|FAIL|
|test-memcmp|0m1,233s|0m1,028s|0m0,883s|0m0,902s|0m1,025s|FAIL|
|test-strcmp|0m1,059s|0m1,244s|0m1,091s|0m1,073s|0m1,025s|FAIL|
|test-u128|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|
|test-u16|0m2,488s|0m4,150s|0m1,910s|0m1,917s|FAIL|FAIL|
|test-u32|0m2,464s|0m3,358s|FAIL|FAIL|FAIL|FAIL|
|test-u32-cmp|0m6,259s|FAIL|FAIL|FAIL|FAIL|FAIL|
|test-u64|0m2,848s|0m1,686s|0m1,078s|0m1,081s|0m4,623s|0m18,123s|
|test-u8|0m4,255s|0m2,488s|0m1,911s|0m1,916s|FAIL|FAIL|

afl++_cmplog_variant has the best solve, but due to the many solve attempts overall fuzzing performance is decreased, as can be seen at [https://www.fuzzbench.com/reports/experimental/2021-01-08-aflpp/](https://www.fuzzbench.com/reports/experimental/2021-01-08-aflpp/).
So it is hard to say which approach is the best ...

## More testcases or fuzzers?

Just add them and send PR.
