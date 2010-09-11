#!/bin/sh
# Use this script before extracting translatable strings from glade files.
sed -i -e 's#<property name="title">#<property name="title" translatable="yes">#' $1
