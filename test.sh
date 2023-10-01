#!/bin/bash
###################
### START OF CONFIG

# default fuzzer setup (e.g. afl++, libfuzzer, honggfuzz, ...)
FUZZER=afl++

# how many seconds to try each testcase, recommended: 10-120
RUNTIME=120

# test a fuzzer in a specific directory? you can put that here
#FUZZER_DIR=~/AFLplusplus/branches/cmplog_variant

### END OF CONFIG
#################

# cmdline processing
test -z "$1" -o "$1" = "-h" && {
  echo "Syntax: $0 [FUZZER [TESTCASE]]"
  echo Fuzzers: afl++, afl++-gcc, afl++-qemu, afl++-frida, honggfuzz, libfuzzer, libafl
  echo Testcase: instead of processing all, process just this one
  exit 0
}
test -n "$1" && FUZZER=$1
DONE=

# fuzzer options
test "$FUZZER" = "afl++" && { 
  export CC=afl-clang-fast
  export CXX=afl-clang-fast++
  export AFL_LLVM_CMPLOG=1
  export AFL_LLVM_DICT2FILE=`pwd`/afl++.dic
  export CMPLOG_LVL=3ATX
  export FUZZER_OPTIONS="-Z"
  DONE=1
}
test "$FUZZER" = "afl++-gcc" && {
  export CC=afl-gcc-fast
  export CXX=afl-g++-fast++
  export AFL_LLVM_CMPLOG=1
  export AFL_LLVM_DICT2FILE=`pwd`/afl++.dic
  export CMPLOG_LVL=3ATX
  export FUZZER_OPTIONS="-Z"
  DONE=1
}
test "$FUZZER" = "afl++lto" && { 
  export CC=afl-clang-lto
  export CXX=afl-clang-lto++
  export AFL_LLVM_CMPLOG=1
  export AFL_LLVM_DICT2FILE=`pwd`/afl++.dic
  export CMPLOG_LVL=3ATX
  export FUZZER_OPTIONS="-Z"
  export FUZZER=afl++
  DONE=1
}
test "$FUZZER" = "afl++-qemu" -o "$FUZZER" = "afl++-frida" && { 
  export CC=clang
  export CXX=clang++
  export CFLAGS=-D__AFL_COMPILER=1
  export CMPLOG_LVL=3ATX
  export FUZZER_OPTIONS="-Z"
  DONE=1
}
test "$FUZZER" = "libfuzzer" && { 
  export CC=clang
  export CXX=clang++
  export CFLAGS="-fsanitize=fuzzer -fsanitize=address"
  export FUZZER_OPTIONS="-use_value_profile=1 -entropic=1 -close_fd_mask=3 $FUZZER_OPTIONS"
  DONE=1
}
test "$FUZZER" = "libafl" && { 
  export CC=libafl_cc
  export CXX=libafl_cxx
  export CFLAGS="-fsanitize=address --libafl"
  #export FUZZER_OPTIONS="-use_value_profile=1 -entropic=1 $FUZZER_OPTIONS"
  echo PATH=$PATH
  DONE=1
}
test "$FUZZER" = "honggfuzz" && {
  export CC=hfuzz-clang
  export CXX=hfuzz-clang++
  DONE=1
}

test -z "$DONE" && { echo Error: invalid fuzzer, allowed are only afl++, afl++-qemu, afl++-frida, libfuzzer, libafl or honggfuzz; exit 1; }
echo Fuzzer: $FUZZER
echo Maximum runtime: $RUNTIME
test -n "$FUZZER_DIR" && echo Detected FUZZER_DIR=$FUZZER_DIR, expanding PATH and AFL_PATH
test -n "$FUZZER_DIR" && export PATH=$FUZZER_DIR:$PATH
test -n "$FUZZER_DIR" && export AFL_PATH=$FUZZER_DIR
echo

# prepare environment
echo Preparation:
export CFLAGS="-O0 -fno-inline -fno-builtin $CFLAGS"
echo CC=$CC
echo CFLAGS=$CFLAGS
env|grep -E '^AFL_'
export CXXFLAGS="$CFLAGS $CXXFLAGS"
export AFL_QUIET=1
make clean >/dev/null 2>&1
test -z "$2" && { make compile || exit 1; }
test -n "$2" && { make "$2" || exit 1; }
rm -rf in out-* *.log crash* SIG* HONGGFUZZ.REPORT.TXT
ulimit -c 0
mkdir in || exit 1
echo AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA > in/in
test "$FUZZER" = "afl++" -o "$FUZZER" = "afl++-qemu" -o "$FUZZER" = "afl++-frida" && {
  OK=
  afl-fuzz -h 2>&1 | grep -q ' -l ' && OK=1
  test -z "$OK" && echo Warning: afl++ is not cmplog_variant
  test -n "$OK" && FUZZER_OPTIONS="-l $CMPLOG_LVL $FUZZER_OPTIONS"
}

# set envs
export AFL_NO_UI=1
export AFL_BENCH_UNTIL_CRASH=1
export AFL_SKIP_CPUFREQ=1
export AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1
export RUNTIME
unset ASAN_FUZZER_OPTIONS
export ASAN_OPTIONS="disable_coredump=0:unmap_shadow_on_exit=1:abort_on_error=1:detect_leaks=0:symbolize=0"
SUCCESS=0
FAIL=0
echo
echo Starting tests
echo Fuzzer special options: $FUZZER_OPTIONS

# run test cases
for i in *.c; do

  TARGET=${i%%.c*}
  test -z "$2" -o "$2" = "$TARGET" && {

    test -x "$TARGET" && {
      echo Running $TARGET ...

      test -e ${AFL_TMPDIR}/.cur_input && rm ${AFL_TMPDIR}/.cur_input
      test "$FUZZER" = afl++ -o "$FUZZER" = afl++-gcc && {
        HAVE_DICT=""
        test -f afl++.dic && HAVE_DICT="-x afl++.dic"
        TIME=`{ time afl-fuzz $HAVE_DICT $FUZZER_OPTIONS -V$RUNTIME -i in -o out-$TARGET -c ./$TARGET -- ./$TARGET >/dev/null 2>$TARGET.log ; } 2>&1 |grep -w real|awk '{print$2}'`
        ls out-$TARGET/default/crashes/id* >/dev/null 2>&1 && {
          echo SUCCESS: $TARGET $TIME
          test -z "$NO_DELETE" && rm -rf out-$TARGET $TARGET.log
          SUCCESS=$((SUCCESS + 1))
        } || {
          echo FAIL: $TARGET
          for xx in out-$TARGET/default/queue/id*; do
            echo -n $xx= ; ./$TARGET $xx
          done 2>&1 | sort -n -k +4
          echo 
          FAIL=$((FAIL + 1))
        }
        test -z "$NO_DELETE" && rm -rf out-$TARGET $TARGET.log
      }

      test "$FUZZER" = afl++-qemu && {
        TIME=`{ time afl-fuzz -Q $FUZZER_OPTIONS -V$RUNTIME -i in -o out-$TARGET -c 0 -- ./$TARGET >/dev/null 2>$TARGET.log ; } 2>&1 |grep -w real|awk '{print$2}'`
        ls out-$TARGET/default/crashes/id* >/dev/null 2>&1 && {
          echo SUCCESS: $TARGET $TIME
          SUCCESS=$((SUCCESS + 1))
        } || {
          echo FAIL: $TARGET
          for xx in out-$TARGET/default/queue/id*; do
            echo -n $xx= ; ./$TARGET $xx
          done 2>&1 | sort -n -k +4
          echo 
          FAIL=$((FAIL + 1))
        }
        test -z "$NO_DELETE" && rm -rf out-$TARGET $TARGET.log
      }

      test "$FUZZER" = afl++-frida && {
        TIME=`{ time afl-fuzz -O $FUZZER_OPTIONS -V$RUNTIME -i in -o out-$TARGET -c 0 -- ./$TARGET >/dev/null 2>$TARGET.log ; } 2>&1 |grep -w real|awk '{print$2}'`
        ls out-$TARGET/default/crashes/id* >/dev/null 2>&1 && {
          echo SUCCESS: $TARGET $TIME
          SUCCESS=$((SUCCESS + 1))
        } || {
          echo FAIL: $TARGET
          for xx in out-$TARGET/default/queue/id*; do
            echo -n $xx= ; ./$TARGET $xx
          done 2>&1 | sort -n -k +4
          echo 
          FAIL=$((FAIL + 1))
        }
        test -z "$NO_DELETE" && rm -rf out-$TARGET $TARGET.log
      }

      test "$FUZZER" = honggfuzz && {
        cp -r in out-$TARGET
        TIME=`{ time honggfuzz $FUZZER_OPTIONS --run_time $RUNTIME -q --exit_upon_crash -i out-$TARGET -s -v -- ./$TARGET >/dev/null 2>$TARGET.log ; } 2>&1 |grep -w real|awk '{print$2}'`
        ls SIG* >/dev/null 2>&1 && {
          echo SUCCESS: $TARGET $TIME
          SUCCESS=$((SUCCESS + 1))
        } || {
          echo FAIL: $TARGET
          for xx in out-$TARGET/*; do
            echo -n $xx= ; ./$TARGET $xx
          done 2>&1 | sort -n -k +4
          echo 
          FAIL=$((FAIL + 1))
        }
        rm -f SIG* HONGGFUZZ.REPORT.TXT
        test -z "$NO_DELETE" && rm -rf out-$TARGET $TARGET.log
      }

      test "$FUZZER" = libfuzzer && {
        cp -r in out-$TARGET
        # -use_value_profile=1 decreases the performance
        TIME=`{ time ./$TARGET $FUZZER_OPTIONS -timeout=1 -detect_leaks=0 -max_total_time=$RUNTIME -workers=0 out-$TARGET in >/dev/null 2>$TARGET.log ; } 2>&1 |grep -w real|awk '{print$2}'`
        ls crash* >/dev/null 2>&1 && {
          echo SUCCESS: $TARGET $TIME
          rm -f crash*
          SUCCESS=$((SUCCESS + 1))
        } || {
          echo FAIL: $TARGET
          for xx in out-$TARGET/*; do
            echo -n $xx= ; ./$TARGET $xx 2>&1|grep wrong|head -n 1
          done | sort -n -k +4
          echo 
          FAIL=$((FAIL + 1))
        }
        test -z "$NO_DELETE" && rm -rf out-$TARGET $TARGET.log
      }

      test "$FUZZER" = libafl && {
        cp -r in out-$TARGET
        TIME=`{ time timeout -s KILL $RUNTIME ./$TARGET $FUZZER_OPTIONS -i in -o out-$TARGET >/dev/null 2>$TARGET.log ; } 2>&1 |grep -w real|awk '{print$2}'`
        ls out-$TARGET/crashes/* >/dev/null 2>&1 && {
          echo SUCCESS: $TARGET $TIME
          rm -f crash*
          SUCCESS=$((SUCCESS + 1))
        } || {
          echo FAIL: $TARGET
          for xx in out-$TARGET/*; do
            echo -n $xx= ; ./$TARGET $xx
          done 2>&1 | sort -n -k +4
          echo 
          FAIL=$((FAIL + 1))
        }
        test -z "$NO_DELETE" && rm -rf out-$TARGET $TARGET.log
      }

    }

  }

done

echo "Done! SUCCESS=$SUCCESS FAIL=$FAIL"
