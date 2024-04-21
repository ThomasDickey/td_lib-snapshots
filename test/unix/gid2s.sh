#!/bin/sh
# $Id: gid2s.sh,v 12.2 2024/04/21 20:24:18 tom Exp $
#
OUT=gid2s.tmp
SED=/tmp/sed$$
REF=/tmp/ref$$
ARG=/tmp/arg$$
#
trap "rm -f $ARG $REF $SED; exit 1" HUP INT QUIT TERM
trap "rm -f $ARG $REF $SED" EXIT
rm -f $OUT
#
head -20 /etc/group |\
	grep -v '[+]' |\
	sed -e 's@:[^:]*:@ @' -e 's@:.*$@@' >$SED
#
if test ! -s $SED	# SunOs puts this in YP
then
	set - `id | sed -e 's@^.*groups=@@' -e s'@)@@g' -e s'@(@ @g' -e s'@,@ @g'`
	while test $# != 0
	do
		echo $2 $1 >>$SED
		shift
		shift
	done
fi
if test ! -s $SED
then	echo '? no groups' >$OUT
	exit
fi
#
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
