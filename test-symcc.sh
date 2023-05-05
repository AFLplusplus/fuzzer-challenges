#!/bin/bash

#
# Setup for SYMCC
#

FILES=`ls test-*.c|sed 's/\.c//'`
export PATH=$PATH:/symcc:/symcc_build

make clean
make CC=cc CFLAGS="-g -D__AFL_COMPILER=1" compile
for i in $FILES; do
  test -x $i && mv $i $i.plain
done
make CC=symcc CFLAGS="-g -D__AFL_COMPILER=1" compile
export SYMCC_OUTPUT_DIR=`pwd`/out

for j in $FILES; do
test -x $j && {

  # Setup individual run
  echo ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ > test.txt
  PREV=-1
  CUR=0
  rm -f core
  i=test.txt

  while [ '!' "$PREV" = "$CUR" -a '!' -f core -a -e test.txt ]; do
    PREV=$CUR
    echo CUR=$CUR FILE=$i
    rm -rf out
    mkdir out
    cat test.txt | ./$j
    mv test.txt $j.data
    FILE=
    for i in out/*; do
      RES=$(./$j.plain $i 2>&1|grep -w at |sed 's/.* //')
      echo FILE=$i RES=$RES CUR=$CUR BEST=$FILE
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

rm -rf out core
echo
echo RESULTS
echo =======
grep CORE *.log
