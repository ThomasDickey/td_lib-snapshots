#!/bin/sh
# $Id: gid2s.sh,v 11.3 1992/11/18 12:41:05 dickey Exp $
#
OUT=gid2s.tmp
SED=/tmp/sed$$
REF=/tmp/ref$$
ARG=/tmp/arg$$
#
trap "rm -f $ARG $REF $SED" 0 1 2 5 15
head -20 /etc/group | sed -e 's@:[^:]*:@ @' -e 's@:.*$@@' >$SED
#
rm -f $OUT
echo '** scanning /etc/group' >$OUT
while read name number
do
	echo $number >>$ARG
	echo $number '=> "'$name'"' >>$REF
done <$SED
#
echo '** scanned /etc/group' >$OUT
./gid2s `cat $ARG` >$SED
echo '** converted /etc/group' >>$OUT
diff $SED $REF >>$OUT
