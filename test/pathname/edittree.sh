#!/bin/sh
# $Id: edittree.sh,v 8.0 1992/11/20 11:15:24 ste_cm Rel $
ME=edittree
#
rm -rf junk
trap "rm -rf junk" 0 1 2 5 15
mkdir junk
mkdir junk/foo
mkdir junk/bar
touch junk/first
touch junk/last
touch junk/bar/moe2
touch junk/bar/mo3
touch junk/bar/moe
./$ME    junk 2>&1  >$ME.tmp 2>&1
./$ME -r junk 2>&1 >>$ME.tmp 2>&1
