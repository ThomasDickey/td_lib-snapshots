#!/bin/sh
# $Id: usercopy.sh,v 12.0 1992/11/18 15:07:52 ste_cm Rel $
ME=usercopy
TMP=/tmp/copy$$
trap "rm -f $TMP" 0 1 2 5 15
./$ME dummy.txt $TMP >/dev/null
if (cmp -s $TMP dummy.txt)
then	echo ok >$ME.tmp
else	diff dummy.txt $TMP >$ME.tmp
fi
