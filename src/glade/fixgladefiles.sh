#!/bin/sh
# Use this script before extracting translatable strings from glade files.
# It changes every title property to translatable.

REGEXP='s#<property name="title">#<property name="title" translatable="yes">#'

for i in *.glade; do
  echo "Fixing: $i"
  sed -i -e "$REGEXP" $i
done
