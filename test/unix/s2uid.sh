#!/bin/sh
# $Id: s2uid.sh,v 12.0 1992/11/24 11:04:27 ste_cm Rel $
#
OUT=s2uid.tmp
SED=/tmp/sed$$
REF=/tmp/ref$$
ARG=/tmp/arg$$
#
trap "rm -f $ARG $REF $SED" 0 1 2 5 15
head -40 /etc/passwd | sed -e 's@:[^:]*:@ @' -e 's@:.*$@@' |fgrep -v . >$SED
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
