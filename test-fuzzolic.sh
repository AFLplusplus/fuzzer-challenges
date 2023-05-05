#!/bin/bash

#
# Setup for Fuzzolic
#
test -f /share/test-u16.c || { echo Error: use the fuzzolic dockerfile and mount fuzzer-challenges to /share; exit 1; }
test -f fuzzolic/fuzzolic.py || { echo Error: use the fuzzolic dockerfile and execute this from /home/ubuntu/fuzzolic; exit 1; }

clang-12 -v || { apt update -y; apt install -y clang-12; }

make -C /share clean
make CC=clang-12 CFLAGS="-g -D__AFL_COMPILER=1" -C /share compile
ulimit -c unlimited

rm -rf in
mkdir in
echo ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ > in/in

# removed longdouble - takes forever
for j in test-crc32 test-transform test-u32 test-u8 test-double test-memcmp test-u128 test-u32-cmp test-float test-strcmp test-u16 test-u64; do
{

  # Setup individual run
  rm -rf core out
  echo Running $j ...
  timeout -s KILL 15 fuzzolic/fuzzolic.py --symbolic-models --memory-slice --address-reasoning --optimistic-solving --timeout 90000 -o out -i in --keep-run-dirs -- /share/$j > $j.log 2>&1
}
done

rm -rf in out

echo
echo RESULTS
echo =======
grep -wl CRASH test-*.log | sed 's/\.log//'
