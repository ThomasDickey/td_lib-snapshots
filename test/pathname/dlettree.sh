#!/bin/sh
# $Id: dlettree.sh,v 7.1 1992/11/20 11:11:27 dickey Exp $
#
ME=deletetree
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
