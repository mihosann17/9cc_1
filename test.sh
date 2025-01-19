#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 "0;"
assert 42 "42;"
assert 21 "5+20-4;"
assert 10 "5-6+11;"
assert 123 "100 + 20 + 3;"
assert 38 "14 + 7 + 20 - 3;"
assert 15 "5 * 3;"
assert 2 "4 / 2;"
assert 5 "(2 + 3);"
assert 7 "1 + 2 * 3;"
assert 5 "1 * 2 + 3;"
assert 8 "2 * ( 1 + 3 );"
assert 1 "+ 1 ;"
assert 10 "-10 + 20;"
assert 1 " 2 == 2;"
assert 0 " 1 == 0;"
assert 1 " 0 != 1;"
assert 0 " 4 != 4;"
assert 1 " 0 <= 1;"
assert 1 " 0 <= 0;"
assert 0 " 1 <= 0;"
assert 1 " 0 < 1 ;"
assert 0 " 4 < 4 ;"
assert 0 " 1 < 0 ;"
assert 1 " 4 >= 3 ;"
assert 1 " 5 >= 5 ;"
assert 0 " 7 >= 9 ;"
assert 1 " 4 > 3 ;"
assert 0 " 4 > 4 ;"
assert 0 " 4 > 5 ;"

assert 1 "a = 1;"
assert 5 "a = 2;b = 3;a + b;"
assert 26 "a=1;b=1;c=1;d=1;e=1;f=1;g=1;h=1;i=1;j=1;k=1;l=1;m=1;n=1;o=1;p=1;q=1;r=1;s=1;t=1;u=1;v=1;w=1;x=1;y=1;z=1;a+b+c+d+e+f+g+h+i+j+k+l+m+n+o+p+q+r+s+t+u+v+w+x+y+z;"
assert 27 "a=1;b=1;c=1;d=1;e=1;f=1;g=1;h=1;i=1;j=1;k=1;l=1;m=1;n=1;o=1;p=1;q=1;r=1;s=1;t=1;u=1;v=1;w=1;x=1;y=1;z=1;aa=1;a+b+c+d+e+f+g+h+i+j+k+l+m+n+o+p+q+r+s+t+u+v+w+x+y+z+aa;"
assert 28 "a=1;b=1;c=1;d=1;e=1;f=1;g=1;h=1;i=1;j=1;k=1;l=1;m=1;n=1;o=1;p=1;q=1;r=1;s=1;t=1;u=1;v=1;w=1;x=1;y=1;z=1;aa=1;ab=1;a+b+c+d+e+f+g+h+i+j+k+l+m+n+o+p+q+r+s+t+u+v+w+x+y+z+aa+ab;"
assert 12 "a=4;return a + a + a;"

#if/else
assert 2 "a=1; if (a==1) return 2;"
assert 2 "a=1; if (a==1) return 2; return 3;"
assert 3 "a=1; if (a!=1) return 2; return 3;"
assert 2 "a=1; if (a==1) a = a + 1; return a;"
assert 2 "a=1; if (a==1) return 2; else a = 3; return a;"
assert 2 "a=3; if (a!=1) return 2; else a = 3; return a;"
assert 2 "a=1; if (a==1) a=2; else a = 3; return a;"

echo OK