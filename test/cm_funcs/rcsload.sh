#!/bin/sh
# $Id: rcsload.sh,v 12.0 1992/11/19 09:17:21 ste_cm Rel $
ME=rcsload
RCS_DEBUG=2;export RCS_DEBUG
RCS_DIR=/tmp;export RCS_DIR
#
rm -f $ME.tmp
for n in dummy3.txt dummy4.txt
do
	rm -f /tmp/$n,v $ME.tst
	trap "rm -f /tmp/$n,v $ME.tst" 0 1 2 5 15
	cp $n /tmp/$n,v
	./$ME $n >>$ME.tmp 2>$ME.tst
	RCS_DEBUG=0; export RCS_DEBUG
	rm -f /tmp/$n,v
done
