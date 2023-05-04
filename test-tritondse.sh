#!/bin/bash

#
# Setup for Manticore
#
make clean
make CC=clang CFLAGS="-g -D__AFL_COMPILER=1" compile
ulimit -c unlimited

# removed longdouble - takes forever
for j in test-crc32 test-transform test-u32 test-u8 test-double test-memcmp test-u128 test-u32-cmp test-float test-strcmp test-u16 test-u64; do
{

  # Setup individual run
  rm -rf core out
  mkdir out
  echo Running $j ...
  timeout -s KILL 15 python3 tritondsetest.py ./$j > $j.log 2>&1
  cd out
  for i in *; do cat $i | ../$j ; done
  test -e core && echo SUCCESS | tee -a ../$j.log
  cd ..
  rm -rf out
}
done

echo
echo RESULTS
echo =======
grep -w SUCCESS test-*.log | sed 's/\.log//'
