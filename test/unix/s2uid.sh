#!/bin/sh
# $Id: s2uid.sh,v 12.1 2022/10/11 08:15:23 tom Exp $
#
OUT=s2uid.tmp
SED=/tmp/sed$$
REF=/tmp/ref$$
ARG=/tmp/arg$$
#
trap "rm -f $ARG $REF $SED" 0 1 2 5 15
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
