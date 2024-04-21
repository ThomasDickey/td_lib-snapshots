#!/bin/sh
# $Id: rcsload.sh,v 12.3 2024/04/21 20:24:18 tom Exp $
ME=rcsload
: ${TMPDIR:-/tmp}
export TMPDIR
RCS_DEBUG=2;export RCS_DEBUG
RCS_DIR=${TMPDIR:-/tmp};export RCS_DIR
#
rm -f $ME.tmp
trap "rm -f /tmp/$N,v $ME.tst; exit 1" HUP INT QUIT TERM
trap "rm -f /tmp/$N,v $ME.tst" EXIT
#
for n in ../dummy3.txt ../dummy4.txt
do
	N=`basename $n`
	rm -f $RCS_DIR/$N,v $ME.tst
	cp $n $RCS_DIR/$N,v
	./$ME $n 2>$ME.tst | sed -e "s,$TMPDIR,TMPDIR,g" >>$ME.tmp
	RCS_DEBUG=0; export RCS_DEBUG
	rm -f /tmp/$N,v
done
