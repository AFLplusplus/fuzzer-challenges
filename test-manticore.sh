#!/bin/bash

#
# Setup for Manticore
#
make clean
echo Warning: needs musl-gcc, otherwise manticore cant decode
make CC=musl-gcc CFLAGS="-static -O0 -g -D__AFL_COMPILER=1" compile

# removed longdouble - takes forever
for j in test-crc32 test-transform test-u32 test-u8 test-double test-memcmp test-u128 test-u32-cmp test-float test-strcmp test-u16 test-u64; do
{

  # Setup individual run
  rm -f core
  echo Running $j ...
  manticore ./$j > $j.log 2>&1

}
done

echo
echo RESULTS
echo =======
for i in test-*.log; do
  grep -w 127 $i | grep -qw exit && echo $i = SUCCESS
done
