make

echo
valgrind ./valgrind_test0.out &> valgrind_test0.res
echo $?
if grep -q "Invalid read" valgrind_test0.res; then
  echo "Invalid read"
fi
if !(grep -q "All heap blocks were freed" valgrind_test0.res); then
  echo "leaks"
fi

echo
valgrind ./valgrind_test1.out &> valgrind_test1.res
echo $?
if grep -q "Invalid read" valgrind_test1.res; then
  echo "Invalid read"
fi
if !(grep -q "All heap blocks were freed" valgrind_test1.res); then
  echo "leaks"
fi

echo
valgrind ./valgrind_test2.out &> valgrind_test2.res
echo $?
if grep -q "Invalid read" valgrind_test2.res; then
  echo "Invalid read"
fi
if !(grep -q "All heap blocks were freed" valgrind_test2.res); then
  echo "leaks"
fi

echo
valgrind ./valgrind_test3.out &> valgrind_test3.res
echo $?
if grep -q "Invalid read" valgrind_test3.res; then
  echo "Invalid read"
fi
if !(grep -q "All heap blocks were freed" valgrind_test3.res); then
  echo "leaks"
fi

#if grep -q $PATTERN $FILE;
