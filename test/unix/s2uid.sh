#!/bin/sh
# $Id: s2uid.sh,v 11.2 1992/11/18 13:34:18 dickey Exp $
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
	echo $name >>$ARG
	echo $name '=>' $number >>$REF
done <$SED
#
echo '** scanned /etc/passwd' >$OUT
./s2uid `cat $ARG` >$SED
echo '** converted /etc/passwd' >>$OUT
diff $SED $REF >>$OUT
