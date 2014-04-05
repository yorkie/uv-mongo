#!/usr/bin/env bash

cpplint='../tools/cpplint.py'
files=''

cd ./src
for line in $(find . -iname '*.[ch]'); do 
  files="$files $line"
done

# run cpplint
echo `$cpplint --extensions=c,h $files`