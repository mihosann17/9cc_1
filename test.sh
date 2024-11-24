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

assert 0 0
assert 42 42
assert 21 "5+20-4"
assert 10 "5-6+11"
assert 123 "100 + 20 + 3"
assert 38 "14 + 7 + 20 - 3"
assert 15 "5 * 3"
assert 2 "4 / 2"
assert 5 "(2 + 3)"
assert 7 "1 + 2 * 3"
assert 5 "1 * 2 + 3"
assert 8 "2 * ( 1 + 3 )"
assert 1 "+ 1 "
assert 10 "-10 + 20"
assert 1 " 2 == 2"
assert 0 " 1 == 0"
assert 1 " 0 != 1"
assert 0 " 4 != 4"
assert 1 " 0 <= 1"
assert 1 " 0 <= 0"
assert 0 " 1 <= 0"
assert 1 " 0 < 1 "
assert 0 " 4 < 4 "
assert 0 " 1 < 0 "
assert 1 " 4 >= 3 "
assert 1 " 5 >= 5 "
assert 0 " 7 >= 9 "
assert 1 " 4 > 3 "
assert 0 " 4 > 4 "
assert 0 " 4 > 5 "

echo OK