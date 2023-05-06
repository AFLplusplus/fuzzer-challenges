# Fuzzing introspection testcases

Test a fuzzer what fuzzing challenges it can solve.
The goal is to have testcases that AFL++ can all complete - once they are all implemented.
Currently the different solving mechanisms have to be set as defined in `src/AFL-fuzz-redqueen.c`.
The comparisons to honggfuzz and libfuzzer are biased, as the testcases are how AFL-fuzz sees things.
The `libfuzzer/` directories has libfuzzer's applicable own testcases, of which AFL++ is only able to solve 64% (vs 86% honggfuzz and 92% libfuzzer).

## Setup

The fuzzers and necessary compilers must be in the path: `AFL-clang-fast`, `AFL-fuzz`, `hfuzz-clang`, `honggfuzz`, `clang`.

## Running

```
./test.sh TARGET [SPECIFIC_TESTCASE]
```

`TARGET` can be (currently) one of:
  * AFL++
  * AFL++-qemu
  * AFL++-frida
  * libAFL (WIP)
  * honggfuzz
  * libfuzzer
  * symsan (via `test-symsan.sh`, use it's docker container)
  * symcc + qemu (via `test-symcc.sh` and `test-symqemu.sh`, use their docker containers)
  * manticore (via `test-manticore.sh`)
  * tritondse (via `test-tritondse.sh`)
  * fuzzolic (via `test-fuzzolic.sh` + docker.io/ercoppa/fuzzolic-runner-v1:ubuntu2004)

Note that manticore and symqemu success can depend on compile options.

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

## Test as of May 2023

All from current repository state (AFL++ is CMPLOG instrumented and `AFL-fuzz -l3AT -Z`).
Solve time: 120 seconds for AFL++/honggfuzz/libfuzzer
Symcc/SymQEMU, symsan, TritonDSE and Manticore are not fuzzers but solvers, hence no time restriction.
SymQEMU currently has zero solves so it has been removed to same space.

|testcase|AFL++|libAFL|symsan|symcc|manticore|tritondse|fuzzolic|AFL++-qemu/frida|honggfuzz-2.5|libfuzzer-13|
|:------:|:---:|:----:|:----:|:---:|:-------:|:-------:|:------:|:--------------:|:-----------:|:----------:|
|test-crc32|0m1,735s|OK|OK|OK|OK|OK|OK|0m14,609s|FAIL|0m14,207s|
|test-double|0m26,823s|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|
|test-extint|0m0,429s|OK|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|
|test-float|0m4,657s|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|
|test-longdouble|0m1,031s|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|
|test-memcmp|0m0,837s|OK|OK|OK|OK|OK|OK|0m6,494s|0m1,005s|0m0,308s|
|test-strcmp|0m0,835s|OK|OK|FAIL|FAIL|OK|FAIL|0m5,727s|0m1,004s|0m1,040s|
|test-transform|0m4,334s(*)|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|FAIL|
|test-u128|0m0,682s|FAIL|FAIL|OK|FAIL|OK|OK|FAIL|FAIL|FAIL|
|test-u16|0m1,252s|OK|OK|OK|OK|OK|OK|0m8,132s|0m1,005s|0m3,741s|
|test-u32|0m0,844s|OK|OK|OK|OK|OK|OK|0m5,185s|0m1,004s|0m2,887s|
|test-u32-cmp|0m1,332s|OK|OK|OK|OK|OK|OK|1m42,470s|0m6,404s|0m0,454s|
|test-u64|0m0,655s|OK|OK|OK|OK|OK|OK|0m3,844s|0m1,005s|0m5,465s|
|test-u8|0m2,263s|OK|OK|OK|OK|OK|OK|0m18,186s|0m1,004s|0m1,370s|

(*) some of these transform solvings are very expensive and hence disabled, but can be enabled at compile time in AFL++

AFL++ has the most solves, but due to the many solve attempts overall fuzzing performance is decreased, as can be seen at [https://www.fuzzbench.com/reports/experimental/2021-01-12-aflpp/](https://www.fuzzbench.com/reports/experimental/2021-01-20-aflpp/).
Interpretation: the **slowest** solver is the best in real-world fuzzing.

## More testcases or fuzzers?

Just add them and send a PR.
