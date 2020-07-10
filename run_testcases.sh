#!/bin/bash

make

exec &> output.txt

VALGRIND=0
if test "$#" -eq 1; then
  if [ "$1" == "valgrind" ]; then
    VALGRIND=1
  fi
fi

for i in {0..8}
do
  ls -lh testcases/example0$i.stdin
  ls -lh testcases/example0$i.wb
  echo
done

for i in {0..8}
do
  echo Testing example0$i
  if test $VALGRIND -eq 1; then
    cat testcases/example0$i.stdin | valgrind --leak-check=full --show-leak-kinds=all ./loesung testcases/example0$i.wb > results/example0$i.stdout
  else
    cat testcases/example0$i.stdin | ./loesung testcases/example0$i.wb > results/example0$i.stdout
  fi
  echo return code: $?
  if ! diff testcases/example0$i.stdout results/example0$i.stdout > results/diff0$i.txt; then
    echo "testcases/example0$i.stdout and results/example0$i.stdout differ"
  fi
  echo
done
