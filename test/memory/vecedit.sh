#!/bin/sh
# $Id: vecedit.sh,v 12.1 1994/07/02 17:58:28 tom Exp $
ME=vecedit
P=$ME.sh
rm -f $ME.tmp
for Q in ../dummy.txt ../dummy2.txt ../dummy.txt $ME.sh
do
	if ( ./$ME $P $Q >/dev/null )
	then	echo $P $Q >>$ME.tmp
	fi
	P=$Q
done
