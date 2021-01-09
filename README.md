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

On success the time to solve is displayed (note that afl++ has a longer startup time than all other fuzzers).

On failure the the generated corpus files are displayed (so you can see how many finds there were). There is also an output log called `TESTCASE.log`.

## Test as of 9th January 2021

All from current repository state (afl++_vX is the cmplog_variant branch with -l option).
Solve time: 120 seconds

|testcase|afl++_v3|afl++_v2|afl++_v1|afl++_dev|afl++_stable|honggfuzz-2.3|libfuzzer-12|
|:------:|:------:|:------:|:------:|:-------:|:----------:|:-----------:|:----------:|
|test-crc32|0m3,782s|0m2,923s|0m56,395s|0m4,021s|0m4,664s|FAIL|0m2,135s|
|test-double|0m8,027s|0m2,241s|0m8,660s|0m1,676s|FAIL|FAIL|FAIL|
|test-float|0m5,834s|0m13,146s|0m17,137s|0m5,224s|FAIL|FAIL|FAIL|
|test-memcmp|0m0,631s|0m0,634s|0m10,185s|0m0,883s|0m0,902s|0m1,025s|FAIL|
|test-strcmp|0m0,676s|0m0,646s|0m6,345s|0m1,091s|0m1,073s|0m1,025s|FAIL|
|test-u128|0m2,311s|0m1,258s|0m11,990s|FAIL|FAIL|FAIL|FAIL|
|test-u16|0m1,855s|0m2,076s|0m20,908s|0m1,910s|0m1,917s|FAIL|FAIL|
|test-u32|0m1,665s|0m1,290s|0m13,367s|FAIL|FAIL|FAIL|FAIL|
|test-u32-cmp|0m3,964s|FAIL|0m18,834s|FAIL|FAIL|FAIL|FAIL|
|test-u64|0m1,665s|0m0,823s|0m7,008s|0m1,078s|0m1,081s|0m8,823s|0m18,123s|
|test-u8|0m1,810s|0m1,446s|0m10,093s|0m1,911s|0m1,916s|1m16,225s|FAIL|

afl++_cmplog_variant has the best solve, but due to the many solve attempts overall fuzzing performance is decreased, as can be seen at [https://www.fuzzbench.com/reports/experimental/2021-01-08-aflpp/](https://www.fuzzbench.com/reports/experimental/2021-01-08-aflpp/).
So it is hard to say which approach is the best ...

## More testcases or fuzzers?

Just add them and send PR.
