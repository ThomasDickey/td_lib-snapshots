#!/bin/sh
# $Id: gid2s.sh,v 12.0 1992/11/24 13:29:01 ste_cm Rel $
#
OUT=gid2s.tmp
SED=/tmp/sed$$
REF=/tmp/ref$$
ARG=/tmp/arg$$
#
trap "rm -f $ARG $REF $SED" 0 1 2 5 15
rm -f $OUT
#
head -20 /etc/group |\
	fgrep -v + |\
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
