#!/bin/bash

# Fix the names of doxygen-generated man page files:
# * Strip "_ Vec" and "_ T _ _" from "Imath_Box_ Vec2_ T _ _.3"
# * and rename "Imath_*" to "Imath::*"

cd $1/man/man3
for file in Imath_*.3;
do
    new=`echo $file | sed -e 's/_ T _ _//g' -e 's/_ Vec//g' -e s/_/::/g`
    /bin/mv "$file" $new
done



