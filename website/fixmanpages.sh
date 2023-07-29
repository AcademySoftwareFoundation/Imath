#!/bin/bash

# Fix the names of doxygen-generated man page files:
# * Strip "_ Vec" and "_ T _ _" from "Imath_Box_ Vec2_ T _ _.3"
# * and rename "Imath_*" to "Imath::*"

if [ -d $1/man/man3 ]; then

  cd $1/man/man3
  echo cd $1/man/man3
  shopt -s nullglob
  for file in Imath_*.3;
  do
    new=`echo $file | sed -e 's/_ T _ _//g' -e 's/_ Vec//g' -e s/_/::/g`
    echo /bin/mv "$file" $new
    /bin/mv "$file" $new
  done
fi
   


