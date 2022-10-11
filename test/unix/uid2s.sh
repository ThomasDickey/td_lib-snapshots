#!/bin/sh
# $Id: uid2s.sh,v 12.1 2022/10/11 08:15:03 tom Exp $
#
OUT=uid2s.tmp
SED=/tmp/sed$$
REF=/tmp/ref$$
ARG=/tmp/arg$$
#
trap "rm -f $ARG $REF $SED" 0 1 2 5 15
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
