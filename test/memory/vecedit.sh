#!/bin/sh
# $Id: vecedit.sh,v 12.0 1992/11/18 15:01:13 ste_cm Rel $
ME=vecedit
P=$ME.sh
rm -f $ME.tmp
for Q in dummy.txt dummy2.txt dummy.txt $ME.sh
do
	if ( ./$ME $P $Q >/dev/null )
	then	echo $P $Q >>$ME.tmp
	fi
	P=$Q
done
