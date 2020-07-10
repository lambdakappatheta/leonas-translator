make

for i in {1..2}
do
  cat test$i.in > test$i.res
  echo >> test$i.res
  ./fread_test.out test$i.in &>> test$i.res
done
