#!/bin/sh
# $Id: uid2s.sh,v 12.0 1992/11/24 11:00:17 ste_cm Rel $
#
OUT=uid2s.tmp
SED=/tmp/sed$$
REF=/tmp/ref$$
ARG=/tmp/arg$$
#
trap "rm -f $ARG $REF $SED" 0 1 2 5 15
head -40 /etc/passwd |\
	sed -e 's@:[^:]*:@ @' -e 's@:.*$@@' |\
	fgrep -v . |
	grep -v ' [0-9]$' >$SED
#
rm -f $OUT
echo '** scanning /etc/passwd' >$OUT
while read name number
do
	echo $number >>$ARG
	echo $number '=> "'$name'"' >>$REF
done <$SED
#
echo '** scanned /etc/passwd' >$OUT
./uid2s `cat $ARG` >$SED
echo '** converted /etc/passwd' >>$OUT
diff $SED $REF >>$OUT
