#!/bin/sh
# $Id: filecopy.sh,v 12.1 1994/07/02 18:31:54 tom Exp $
ME=filecopy
TMP=/tmp/copy$$
trap "rm -f $TMP" 0 1 2 5 15
./$ME ../dummy.txt $TMP >/dev/null
if (cmp -s $TMP ../dummy.txt)
then	echo ok >$ME.tmp
else
	diff ../dummy.txt $TMP >$ME.tmp
fi
