#!/bin/sh
# $Id: filecopy.sh,v 12.2 2024/04/21 20:24:18 tom Exp $
ME=filecopy
TMP=/tmp/copy$$
trap "rm -f $TMP; exit 1" HUP INT QUIT TERM
trap "rm -f $TMP" EXIT
./$ME ../dummy.txt $TMP >/dev/null
if (cmp -s $TMP ../dummy.txt)
then	echo ok >$ME.tmp
else
	diff ../dummy.txt $TMP >$ME.tmp
fi
