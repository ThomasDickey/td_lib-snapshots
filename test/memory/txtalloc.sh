#!/bin/sh
# $Id: txtalloc.sh,v 12.0 1992/11/18 13:10:12 ste_cm Rel $
OUT=txtalloc.tmp
N=""
for n in aaa bb aaa zzz cc xyz zzzz zabc abc
do
	N="$N $n"
	echo '** added '$n >>$OUT
	./txtalloc $N >>$OUT
done
