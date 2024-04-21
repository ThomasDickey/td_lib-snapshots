#!/bin/sh
# $Id: uid2s.sh,v 12.2 2024/04/21 20:24:18 tom Exp $
#
OUT=uid2s.tmp
SED=/tmp/sed$$
REF=/tmp/ref$$
ARG=/tmp/arg$$
#
trap "rm -f $ARG $REF $SED; exit 1" HUP INT QUIT TERM
trap "rm -f $ARG $REF $SED" EXIT
head -40 /etc/passwd |\
	sed -e 's@:[^:]*:@ @' -e 's@:.*$@@' |\
	grep -v '[.]' |
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
