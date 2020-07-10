make -C ..
echo

if  [ "$1" == "1" ]; then
  for file in man/*.wb
  do
    echo $file
    cat $file > $file.res
    echo "" >> $file.res
    cat input0.in | valgrind ../loesung $file &>> $file.res
    echo $?
    if grep -q "Invalid read" $file.res; then
      echo "Invalid read"
    fi
    if grep -q "Invalid write" $file.res; then
      echo "Invalid write"
    fi
    if !(grep -q "All heap blocks were freed" $file.res); then
      echo "leaks"
    fi
    echo
  done
fi

if  [ "$2" == "1" ]; then
  for file in auto/*.wb
  do
    echo $file
    cat $file > $file.res
    echo "" >> $file.res
    cat input0.in | valgrind ../loesung $file &>> $file.res
    echo $?
    if grep -q "Invalid read" $file.res; then
      echo "Invalid read"
    fi
    if grep -q "Invalid write" $file.res; then
      echo "Invalid write"
    fi
    if !(grep -q "All heap blocks were freed" $file.res); then
      echo "leaks"
    fi
    echo
  done
fi
