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
  * `text-extint` - llvm _ExtInt() tests
  * `test-float` - several chained float checks
  * `test-double` - several chained double checks
  * `test-longdouble` - several chained long double checks
  * `test-memcmp` - several chained memcmp checks
  * `test-strcmp` - several chained strncasecmp checks
  * `test-transform` - different transforming string checks
  * `test-crc32` - several chained crc32 checks

The testcases are still being further refined, some of them were too easy to solve just accidently.

## Results

On success the time to solve is displayed (note that afl++ has a longer startup time than all other fuzzers).

On failure the the generated corpus files are displayed (so you can see how many finds there were). There is also an output log called `TESTCASE.log`.

## Test as of 11th January 2021

All from current repository state (afl++_vX is the cmplog_variant branch with -l option).
Solve time: 120 seconds

|testcase|afl++_v3|afl++_v2|afl++_v1|afl++_dev|afl++_stable|honggfuzz-2.3|libfuzzer-12|
|:------:|:------:|:------:|:------:|:-------:|:----------:|:-----------:|:----------:|
|test-crc32|0m5,596s|0m3,708s|0m58,924s|0m5,477s|0m5,251s|FAIL|FAIL|
|test-double|0m1,032s|0m15,414s|1m14,064s|FAIL|FAIL|FAIL|FAIL|
|test-extint|0m1,663s|0m1,051s|0m6,988s|FAIL|FAIL|FAIL|FAIL|
|test-float|0m1,650s|0m34,206s|0m47,023s|FAIL|FAIL|FAIL|FAIL|
|test-longdouble|0m1,241s|0m1,651s|0m15,159s|FAIL|FAIL|FAIL|FAIL|
|test-memcmp|0m0,622s|0m0,835s|0m7,031s|0m0,893s|0m1,087s|0m1,023s|0m0,089s|
|test-strcmp|0m0,623s|0m0,624s|0m4,745s|FAIL|0m1,091s|0m1,220s|0m13,757s|
|test-transform|FAIL|FAIL|FAIL|FAIL|FAIL|0m31,420s|FAIL|
|test-u128|0m0,418s|0m0,416s|0m2,272s|FAIL|FAIL|FAIL|FAIL|
|test-u16|0m1,233s|0m1,442s|0m18,811s|0m1,928s|0m2,135s|FAIL|FAIL|
|test-u32|0m1,434s|0m1,641s|0m12,085s|FAIL|FAIL|FAIL|FAIL|
|test-u32-cmp|0m1,068s|0m14,712s|0m16,021s|FAIL|FAIL|FAIL|FAIL|
|test-u64|0m0,623s|0m0,625s|0m13,063s|0m1,083s|0m0,896s|FAIL|1m24,684s|
|test-u8|0m1,449s|0m1,259s|0m8,260s|0m2,130s|0m2,139s|FAIL|FAIL|

afl++_cmplog_variant has the most solves, but due to the many solve attempts overall fuzzing performance is decreased, as can be seen at [https://www.fuzzbench.com/reports/experimental/2021-01-08-aflpp/](https://www.fuzzbench.com/reports/experimental/2021-01-08-aflpp/).
Interpretation: the **slowest** solver is the best in real-world fuzzing.

## More testcases or fuzzers?

Just add them and send PR.
