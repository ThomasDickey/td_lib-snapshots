#!/bin/sh
# $Id: rcsload.sh,v 11.2 1992/11/18 15:28:25 dickey Exp $
ME=rcsload
RCS_DEBUG=2;export RCS_DEBUG
RCS_DIR=/tmp;export RCS_DIR
for n in dummy3.txt dummy4.txt
do
	trap "rm -f /tmp/$n /tmp/$n,v" 0 1 2 5 15
	cp $n /tmp/$n,v
	./$ME $n >$ME.tmp
	tb
done
