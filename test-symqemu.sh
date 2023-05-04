#!/bin/bash

test -z "$CC" && CC=clang-11
#
# Setup for SYMQEMU
#
make CC=$CC CFLAGS="-O0 -g -D__AFL_COMPILER=1" compile
export SYMCC_OUTPUT_DIR=`pwd`/out
export PATH=$PATH:/symcc/symqemu/x86_64-linux-user:/symcc


for j in test-crc32 test-longdouble test-transform test-u32 test-u8 test-double test-memcmp test-u128 test-u32-cmp test-float test-strcmp test-u16 test-u64; do
{

  # Setup individual run
  echo ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ > test.txt
  PREV=-1
  CUR=0
  rm -f core
  i=test.txt
  rm -rf "$SYMCC_OUTPUT_DIR"
  mkdir "$SYMCC_OUTPUT_DIR"

  while [ '!' "$PREV" = "$CUR" -a '!' -f core -a -e test.txt ]; do
    PREV=$CUR
    echo CUR=$CUR FILE=$i
    rm -rf out
    mkdir out
    cat test.txt | symqemu-x86_64 ./$j
    mv test.txt $j.data
    FILE=
    for i in out/*; do
      RES=$(./$j $i 2>&1|grep -w at |sed 's/.* //')
      #echo FILE=$i RES=$RES CUR=$CUR BEST=$FILE
      test -n "$RES" && { 
        test "$RES" -gt "$CUR" && { CUR=$RES; FILE=$i; }
      }
    done
    test -n "$FILE" && cp $FILE ./test.txt
  done

  test -e core && echo CORE found
  test -e test.txt && mv test.txt $j.data

} | tee $j.log
done

rm -rf out core "$SYMCC_OUTPUT_DIR"

echo
echo RESULTS
echo =======
grep CORE *.log
