#!/bin/sh
# $Id: rcsload.sh,v 12.1 1994/07/02 17:23:23 tom Exp $
ME=rcsload
RCS_DEBUG=2;export RCS_DEBUG
RCS_DIR=/tmp;export RCS_DIR
#
rm -f $ME.tmp
for n in ../dummy3.txt ../dummy4.txt
do
	rm -f /tmp/$N,v $ME.tst
	N=`basename $n`
	trap "rm -f /tmp/$N,v $ME.tst" 0 1 2 5 15
	cp $n /tmp/$N,v
	./$ME $n >>$ME.tmp 2>$ME.tst
	RCS_DEBUG=0; export RCS_DEBUG
	rm -f /tmp/$N,v
done
