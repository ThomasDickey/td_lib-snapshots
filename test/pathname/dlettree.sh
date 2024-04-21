#!/bin/sh
# $Id: dlettree.sh,v 8.1 2024/04/21 20:24:18 tom Exp $
#
ME=dlettree
#
rm -rf junk
trap "rm -rf junk; exit 1" HUP INT QUIT TERM
trap "rm -rf junk" EXIT
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
