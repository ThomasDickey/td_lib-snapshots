#!/bin/sh
# $Id: s2uid.sh,v 12.2 2024/04/21 20:24:18 tom Exp $
#
OUT=s2uid.tmp
SED=/tmp/sed$$
REF=/tmp/ref$$
ARG=/tmp/arg$$
#
trap "rm -f $ARG $REF $SED; exit 1" HUP INT QUIT TERM
trap "rm -f $ARG $REF $SED" EXIT
head -40 /etc/passwd | sed -e 's@:[^:]*:@ @' -e 's@:.*$@@' |grep -v '[.]' >$SED
#
rm -f $OUT
echo '** scanning /etc/passwd' >$OUT
while read name number
do
	if test "$name" != "+"
	then
		echo $name >>$ARG
		echo $name '=>' $number >>$REF
	fi
done <$SED
#
echo '** scanned /etc/passwd' >$OUT
./s2uid `cat $ARG` >$SED
echo '** converted /etc/passwd' >>$OUT
diff $SED $REF >>$OUT
